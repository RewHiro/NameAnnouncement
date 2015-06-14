#include "cinder/app/AppNative.h"
#include "Name.h"
#include "Background.h"
#include "Key.h"
#include "StarManager.h"

#include "cinder/gl/gl.h"
#include "cinder\Camera.h"
#include "cinder\Json.h"
#include "cinder\gl\Light.h"
#include "cinder\gl\Texture.h"
#include "cinder\ImageIo.h"

#include <memory>

//　stringをwstringに変換する関数
//　@param {string} [str] - 変換したいマルチバイト文字列
//　@param {wstring} [dest] - 変換するwstring
void widen(const std::string& str,std::wstring& dest)
{
	wchar_t* wcs = new wchar_t[str.length() + 1];
	mbstowcs(wcs, str.c_str(), str.length() + 1);
	dest = wcs;
	delete[]wcs;
}

using namespace ci;
using namespace ci::app;
using namespace input;

//　文字コードはshift_jis

class NameAnnouncementApp : public AppNative {

	CameraPersp _camera;
	std::vector<std::shared_ptr<Name>>_namelist;

	std::unique_ptr<gl::Light>light;

	gl::Texture background_texture;

	enum class State
	{
		WAIT,
		DRAIN,
		ANNOUNCEMENT
	};

	StarManager _star_manager;

	State _state = State::WAIT;

	int num = 0;
	int count = 0;
	int index = 0;

	void drain();
	void wait();
	void announce();
  public:
	void setup()final;
	void mouseDown( MouseEvent event )final;	
	void update()final;
	void draw()final;
	void keyDown(KeyEvent event)final;
	void keyUp(KeyEvent event)final;
};

void NameAnnouncementApp::drain()
{
	if (_state != State::DRAIN)return;
	--count;

	if (count == 0)
	{
		num = 0;
		index = randInt(0, _namelist.size());
		_namelist.at(index)->setAnnounce();
		_state = State::ANNOUNCEMENT;

		_star_manager.setAnnnoucement();
	}
}

void NameAnnouncementApp::wait()
{
	if (_state != State::WAIT)return;

	const auto& key = Key::getInstance();

	std::sort(_namelist.begin(), _namelist.end(), [](const std::weak_ptr<Name>& rth, const std::weak_ptr<Name>& lth)
	{
		if (rth.lock()->getPos().z > lth.lock()->getPos().z)return true;
		return false;
	});

	for (auto& name_object : _namelist)
	{
		name_object->update();
	}

	if (!key.isPush(KeyEvent::KEY_RETURN))return;
	for (auto& name_object : _namelist)
	{
		auto time = easeInQuart(static_cast<float>(num) / _namelist.size()) * 1.0f;
		name_object->startDirection(time);
		num++;
	}
	_star_manager.setDrain();
	count = 60 * 3;
	_state = State::DRAIN;
}

void NameAnnouncementApp::announce()
{
	if (_state != State::ANNOUNCEMENT)return;
	auto& key = Key::getInstance();

	if (key.isPush(KeyEvent::KEY_RETURN))
	{
		_namelist.erase(std::remove_if(_namelist.begin(), _namelist.end(), [](const std::weak_ptr<Name>& name)
		{
			return name.lock()->isDelete();
		}),
			_namelist.end()
			);

		_state = State::WAIT;

		for(auto& name : _namelist)
		{
			name->reset();
		}

		_star_manager.reset();
	}
	key.flush();
}


void NameAnnouncementApp::setup()
{

	Key::getInstance();
	
	_star_manager.setup();

	background_texture = loadImage(loadAsset("background.png"));

	light = std::make_unique<gl::Light>(gl::Light::DIRECTIONAL, 0);
	light->setDirection(Vec3f(0,0,-1));
	light->setAmbient(ColorA::white());
	light->setDiffuse(ColorA::white());

	setlocale(LC_CTYPE, "");

	Name::setFont("naguri.ttf", 200);

	JsonTree load(loadAsset("test.json"));

	const auto& objects = load["Name"];

	for (size_t i = 0; i < objects.getNumChildren();++i)
	{
		std::wstring wstr;
		widen(objects[i].getValue<std::string>(), wstr);
		_namelist.emplace_back(std::make_shared<Name>(wstr));
	}
	_camera.setPerspective(90, getWindowAspectRatio(), 0.5f, 1000.0f);
	_camera.lookAt(Vec3f::zAxis() * -10, Vec3f::zero(), Vec3f::yAxis());

	gl::enable(GL_LIGHTING);
	gl::enable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
}

void NameAnnouncementApp::mouseDown( MouseEvent event )
{

}

void NameAnnouncementApp::update()
{
	_star_manager.update();
	announce();
	wait();
	drain();
}

void NameAnnouncementApp::draw()
{
	gl::clear(Color(0, 0, 0));
	light->enable();
	gl::color(ColorA(1, 1, 1));
	gl::draw(background_texture, Rectf(0, 0, static_cast<float>(getWindowWidth()), static_cast<float>(getWindowHeight())));
	gl::pushMatrices();
	gl::setMatrices(_camera);

	gl::enableAdditiveBlending();
	_star_manager.draw();
	gl::disableAlphaBlending();

	gl::enableAlphaBlending();
	for (auto& name_object : _namelist)
	{
		name_object->draw();
	}

	gl::popMatrices();
	gl::disableAlphaBlending();

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
