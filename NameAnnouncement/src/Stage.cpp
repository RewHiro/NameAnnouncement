#include "Stage.h"
#include "Key.h"
#include "Utility.h"

#include "cinder\Json.h"
#include "cinder\Rand.h"

using namespace ci;
using namespace ci::app;
using namespace input;




Stage::Stage():
_light(std::make_unique<gl::Light>(gl::Light::DIRECTIONAL, 0)),
_background_texture(loadImage(loadAsset("textures/background.png")))
{
	setlocale(LC_CTYPE, "");

	_star_manager.setup();
	_light->setDirection(Vec3f(0, 0, -1));
	_light->setAmbient(ColorA::white());
	_light->setDiffuse(ColorA::white());

	std::string name_list_root = "name_list/";
	std::string extension = ".json";

	JsonTree load(loadAsset(name_list_root + getClassName() + extension));

	const auto& objects = load["Name"];

	for (size_t i = 0; i < objects.getNumChildren(); ++i)
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

void Stage::drain()
{
	if (_state != State::DRAIN)return;
	--_count;

	if (_count != 0) return;

	_num = 0;
	_index = randInt(0, _namelist.size());
	_namelist.at(_index)->setAnnounce();
	_state = State::ANNOUNCEMENT;

	_star_manager.setAnnnoucement();
}

void Stage::wait()
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
		auto time = easeInQuart(static_cast<float>(_num) / _namelist.size()) * 1.0f;
		name_object->startDirection(time);
		_num++;
	}
	_star_manager.setDrain();
	_count = 60 * 3;
	_state = State::DRAIN;
}

void Stage::announce()
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

		for (auto& name : _namelist)
		{
			name->reset();
		}

		_star_manager.reset();
	}
	key.flush();
}

void Stage::transiton()
{
	if (!_namelist.empty())return;
	const auto& key = Key::getInstance();
	if (!key.isPush(KeyEvent::KEY_RETURN))return;
	type = SceneType::TITLE;
}

SceneType Stage::update()
{
	_star_manager.update();
	announce();
	wait();
	drain();
	transiton();
	return type;
}

void Stage::draw()
{
	_light->enable();
	gl::color(ColorA(1, 1, 1));
	gl::draw(_background_texture, Rectf(0, 0, static_cast<float>(getWindowWidth()), static_cast<float>(getWindowHeight())));
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
}

void Stage::resize()
{

}