#include "Star.h"
#include "cinder\gl\gl.h"
#include "StarManager.h"
#include "cinder\Rand.h"
#include "cinder\app\App.h"

using namespace ci;
using namespace ci::app;

Star::Star():
_state(State::WAIT),
_color(ColorA(1,1,1)),
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
	_color.a = std::fabs(std::sinf(_color_radian)) * 0.6f;
	_color_radian += 0.01f;
}

void Star::draw()
{
	gl::pushModelView();

	gl::translate(_pos.value().x, _pos_y.value(),_pos.value().z);
	gl::color(_color);
	gl::draw(StarManager::getTexture(), Area(-10, -10, 10, 10));

	gl::color(ColorA(0,1,0));
	gl::draw(StarManager::getTexture(), Area(-10, -10, 10, 10));


	gl::popModelView();
}