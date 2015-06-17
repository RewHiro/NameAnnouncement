#include "cinder/app/AppNative.h"
#include "Name.h"
#include "Background.h"
#include "Key.h"
#include "StarManager.h"
#include "SceneManager.h"
#include "Stage.h"

#include "cinder/gl/gl.h"
#include "cinder\Camera.h"
#include "cinder\Json.h"
#include "cinder\gl\Light.h"
#include "cinder\gl\Texture.h"
#include "cinder\ImageIo.h"
#include "cinder\Rand.h"

#include <memory>



using namespace ci;
using namespace ci::app;
using namespace input;

//　文字コードはshift_jis

class NameAnnouncementApp : public AppNative {

	std::unique_ptr<SceneManager>_scene_manager;
  public:
	void prepareSettings(Settings *settings)final;
	void setup()final;
	void shutdown();
	void mouseDown( MouseEvent event )final;	
	void update()final;
	void draw()final;
	void keyDown(KeyEvent event)final;
	void keyUp(KeyEvent event)final;
};

void NameAnnouncementApp::prepareSettings(Settings *settings)
{
	JsonTree load(loadAsset("window.json"));
	const auto& objects = load["Window"];
	settings->setWindowSize(objects["SizeX"].getValue<int>(), objects["SizeY"].getValue<int>());
	SceneManager::getWindowSize() = Vec2f(objects["SizeX"].getValue<float>(), objects["SizeY"].getValue<float>());
	settings->setFrameRate(60);
}

void NameAnnouncementApp::setup()
{
	_scene_manager = std::make_unique<SceneManager>();
	Key::getInstance();

	setlocale(LC_CTYPE, "");

	JsonTree load(loadAsset("font/font.json"));
	const auto& objects = load["Font"];
	auto font_name = objects["Name"].getValue<std::string>();
	std::string font_root = "font/";
	Name::setFont(font_root + font_name, 100);
	Name::getFontScele() = objects["Size"].getValue<float>();

	randSeed(std::random_device()());

	JsonTree sound_load(loadAsset("sound/sound.json"));

	std::string sound_root = "sound/";
	auto drum_se_filename = sound_load["DrainSE"]["FileName"].getValue<std::string>();
	auto announce_se_filename = sound_load["AnnounceSE"]["FileName"].getValue<std::string>();

	Stage::getDrumSE() = audio::Voice::create(audio::load(loadAsset(sound_root + drum_se_filename)));
	Stage::getDrumSE()->setVolume(sound_load["DrainSE"]["Volume"].getValue<float>());

	Stage::getAnnounceSE() = audio::Voice::create(audio::load(loadAsset(sound_root + announce_se_filename)));
	Stage::getAnnounceSE()->setVolume(sound_load["AnnounceSE"]["Volume"].getValue<float>());

	JsonTree effect_load(loadAsset("effect_time.json"));
	Stage::getDrainTime() = effect_load["DrainOffset"].getValue<float>();
	Name::getBounceTime() = effect_load["Bounce"].getValue<float>();
	Stage::getInterbalTime() = effect_load["DrainToAnnouceInterbal"].getValue<int>();
	
}

void NameAnnouncementApp::shutdown()
{
	_scene_manager->shutdown();
}

void NameAnnouncementApp::mouseDown( MouseEvent event )
{

}

void NameAnnouncementApp::update()
{
	_scene_manager->update();
	if (!Key::getInstance().isPush(KeyEvent::KEY_ESCAPE))return;
	quit();
}

void NameAnnouncementApp::draw()
{
	gl::clear(Color(0, 0, 0));
	_scene_manager->draw();
	Key::getInstance().flush();
}


void NameAnnouncementApp::keyDown(KeyEvent event)
{
	Key::getInstance().setKeyDown(event.getCode());
}

void NameAnnouncementApp::keyUp(KeyEvent event)
{
	Key::getInstance().setKeyUp(event.getCode());
}

CINDER_APP_NATIVE( NameAnnouncementApp, RendererGl )
