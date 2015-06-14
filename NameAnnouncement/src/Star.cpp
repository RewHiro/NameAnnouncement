#include "Star.h"
#include "cinder\gl\gl.h"
#include "StarManager.h"
#include "cinder\Rand.h"
#include "cinder\app\App.h"

using namespace ci;
using namespace ci::app;

Star::Star():
_state(State::WAIT),
_emission_color(ColorA(1, 1, 1)),
_color(ColorA(0, 1, 0)),
_pos(Anim<Vec3f>(Vec3f
(
randFloat(-getWindowCenter().x, getWindowCenter().x),
randFloat(-getWindowCenter().y, getWindowCenter().y),
randFloat(200.0f,800.0f)
)
)),
_pos_y(Anim<float>(_pos.value().y)),
_color_radian(randFloat(0.0f,1.07f))
{
	timeline().apply(
		&_pos,
		_pos.value(),
		Vec3f(randFloat(-getWindowCenter().x, getWindowCenter().x),
		randFloat(-getWindowCenter().y, getWindowCenter().y),
		randFloat(200.0f, 800.0f)),
		10.0f,
		EaseOutElastic(30.0f, 5.0f)
		).finishFn(std::bind(&Star::burstOut, this));

	timeline().apply(
		&_pos_y,
		_pos_y.value(),
		randFloat(-getWindowCenter().y, getWindowCenter().y),
		10.0f,
		EaseOutElastic(30.0f, 5.0f)
		).finishFn(std::bind(&Star::burstOut, this));
}

void Star::shake()
{

	std::map<int, std::function<void()>>easing_map =
	{
		{ 0, std::bind(&Star::burstIn, this) },
		{ 1, std::bind(&Star::burstInOut, this) },
		{ 2, std::bind(&Star::burstOut, this) },
		{ 3, std::bind(&Star::slowmove, this) },
	};

	timeline().apply(
		&_pos,
		_pos.value(),
		Vec3f(randFloat(-getWindowCenter().x, getWindowCenter().x),
		randFloat(-getWindowCenter().y, getWindowCenter().y),
		randFloat(200.0f, 800.0f)),
		10.0f,
		EaseInSine()
		).finishFn(easing_map.at(randInt(0, 3)));

	timeline().apply(
		&_pos_y,
		_pos_y.value(),
		randFloat(-getWindowCenter().y, getWindowCenter().y),
		10.0f,
		EaseInOutBack(randFloat(1.7f, 4.0f))
		).finishFn(easing_map.at(randInt(0, 3)));
}

void Star::burstIn()
{
	timeline().apply(
		&_pos,
		_pos.value(),
		Vec3f(randFloat(-getWindowCenter().x, getWindowCenter().x),
		randFloat(-getWindowCenter().y, getWindowCenter().y),
		randFloat(200.0f, 800.0f)),
		10.0f,
		EaseInElastic(30.0f, 5.0f)
		).finishFn(std::bind(&Star::slowmove, this));

	timeline().apply(
		&_pos_y,
		_pos_y.value(),
		randFloat(-getWindowCenter().y, getWindowCenter().y),
		10.0f,
		EaseInElastic(30.0f, 5.0f)
		).finishFn(std::bind(&Star::slowmove, this));
}
void Star::burstOut()
{
	timeline().apply(
		&_pos,
		_pos.value(),
		Vec3f(randFloat(-getWindowCenter().x, getWindowCenter().x),
		randFloat(-getWindowCenter().y, getWindowCenter().y),
		randFloat(200.0f, 800.0f)),
		10.0f,
		EaseOutElastic(30.0f, 5.0f)
		).finishFn(std::bind(&Star::slowmove, this));

	timeline().apply(
		&_pos_y,
		_pos_y.value(),
		randFloat(-getWindowCenter().y, getWindowCenter().y),
		10.0f,
		EaseOutElastic(30.0f, 5.0f)
		).finishFn(std::bind(&Star::slowmove, this));
}
void Star::burstInOut()
{
	timeline().apply(
		&_pos,
		_pos.value(),
		Vec3f(randFloat(-getWindowCenter().x, getWindowCenter().x),
		randFloat(-getWindowCenter().y, getWindowCenter().y),
		randFloat(200.0f, 800.0f)),
		10.0f,
		EaseInOutElastic(30.0f, 5.0f)
		).finishFn(std::bind(&Star::slowmove, this));

	timeline().apply(
		&_pos_y,
		_pos_y.value(),
		randFloat(-getWindowCenter().y, getWindowCenter().y),
		10.0f,
		EaseInOutElastic(30.0f, 5.0f)
		).finishFn(std::bind(&Star::slowmove, this));
}

void Star::slowmove()
{
	std::map<int, std::function<void()>>easing_map =
	{
		{ 0, std::bind(&Star::burstIn, this) },
		{ 1, std::bind(&Star::burstInOut, this) },
		{ 2, std::bind(&Star::burstOut, this) },
		{ 3, std::bind(&Star::shake, this) },
	};

	timeline().apply(
		&_pos,
		_pos.value(),
		Vec3f(randFloat(-getWindowCenter().x, getWindowCenter().x),
		randFloat(-getWindowCenter().y, getWindowCenter().y),
		randFloat(200.0f, 800.0f)),
		10.0f,
		EaseOutCubic()
		).finishFn(easing_map.at(randInt(0, 3)));

	timeline().apply(
		&_pos_y,
		_pos_y.value(),
		randFloat(-getWindowCenter().y, getWindowCenter().y),
		10.0f,
		EaseOutCubic()
		).finishFn(easing_map.at(randInt(0, 3)));
}


void Star::update()
{
	_emission_color.a = std::fabs(std::sinf(_color_radian)) * 0.6f;
	_color_radian += 0.01f;
	if(_state == State::ANNOUNCE)
	{
		_emission_color.a = std::fabs(std::sinf(_color_radian));
	}
}

void Star::draw()
{
	gl::pushModelView();

	gl::translate(_pos.value().x, _pos_y.value(),_pos.value().z);

	gl::color(_emission_color);
	gl::draw(StarManager::getTexture(), Area(-10, -10, 10, 10));

	gl::color(_color);
	gl::draw(StarManager::getTexture(), Area(-10, -10, 10, 10));


	gl::popModelView();
}

void Star::setDrain()
{
	_pos.stop();
	_pos_y.stop();
	_state = State::DRAIN;
	timeline().apply(&_pos, _pos.value(), Vec3f(0,0,1000), 1.5f, easeInExpo);
	timeline().apply(&_pos_y, _pos_y.value(), 0.0f, 1.5f, easeInExpo);
}

void Star::setAnnouncement()
{
	_pos.stop();
	_pos_y.stop();
	_state = State::ANNOUNCE;

	
	_color.r = randFloat();
	_color.g = randFloat();
	_color.b = randFloat();

	timeline().apply(
		&_pos, 
		_pos.value(), 
		Vec3f(
		randFloat(-getWindowCenter().x, getWindowCenter().x),
		randFloat(-getWindowCenter().y, getWindowCenter().y),
		randFloat(100.0f, 800.0f)
		), 
		2.0f, 
		easeInOutExpo
		);

	timeline().apply(
		&_pos_y, 
		_pos_y.value(),
		randFloat(-getWindowCenter().y, getWindowCenter().y),
		2.0f,
		easeInOutExpo
		);
}

void Star::reset()
{
	_pos.stop();
	_pos_y.stop();

	_color.r = 0.0f;
	_color.g = 1.0f;
	_color.b = 0.0f;

	_state = State::WAIT;


	timeline().apply(
		&_pos,
		_pos.value(),
		Vec3f(randFloat(-getWindowCenter().x, getWindowCenter().x),
		randFloat(-getWindowCenter().y, getWindowCenter().y),
		randFloat(200.0f, 800.0f)),
		10.0f,
		EaseOutElastic(30.0f, 5.0f)
		).finishFn(std::bind(&Star::burstOut, this));

	timeline().apply(
		&_pos_y,
		_pos_y.value(),
		randFloat(-getWindowCenter().y, getWindowCenter().y),
		10.0f,
		EaseOutElastic(30.0f, 5.0f)
		).finishFn(std::bind(&Star::burstOut, this));
}