#include "cinder/app/AppNative.h"
#include "Name.h"
#include "Background.h"
#include "Key.h"
#include "StarManager.h"
#include "SceneManager.h"

#include "cinder/gl/gl.h"
#include "cinder\Camera.h"
#include "cinder\Json.h"
#include "cinder\gl\Light.h"
#include "cinder\gl\Texture.h"
#include "cinder\ImageIo.h"

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
	Name::setFont(font_root + font_name, objects["Size"].getValue<float>());

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
