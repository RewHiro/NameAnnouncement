#include "Name.h"
#include "cinder\gl\gl.h"
#include "cinder\Unicode.h"
#include "cinder\Rand.h"
#include "Key.h"


using namespace ci;
using namespace ci::app;
using namespace input;

Name::Name(const std::wstring& name):
_state(State::WAIT),
_name(name),
_pos(Anim<Vec3f>(Vec3f(
randFloat(-getWindowCenter().x * 3, getWindowCenter().x * 3),
randFloat(-getWindowCenter().y * 3, getWindowCenter().y * 3),
800.0f
))),
_scale(Anim<Vec3f>(Vec3f::one())),
_speed(Vec3f(
randFloat(-1,1),
randFloat(-1, 1),
randFloat(-2, 2)
))
{
}

void Name::wait()
{
	if (_state != State::WAIT)return;
	_pos.value() += _speed;

	//　上限
	static const Vec3f SUPREMUM =
		Vec3f(
		getWindowCenter().x + 100,
		getWindowCenter().y + 100,
		900.0f
		);
	//　下限
	static const Vec3f INFIMUM =
		Vec3f(
		-getWindowCenter().x - 100,
		-getWindowCenter().y - 100,
		700.0f
		);;

	if(_pos.value().x > SUPREMUM.x || _pos.value().x < INFIMUM.x)
	{

		_speed.x *= -1;
	}
	if (_pos.value().y > SUPREMUM.y || _pos.value().y < INFIMUM.y)
	{
		_speed.y *= -1;
	}
	if (_pos.value().z > SUPREMUM.z || _pos.value().z < INFIMUM.z)
	{
		_speed.z *= -1;
	}
}

void Name::reset()
{
	_pos.stop();
	_scale.stop();

	_pos = Anim<Vec3f>(Vec3f(
		randFloat(-getWindowCenter().x * 3, getWindowCenter().x * 3),
		randFloat(-getWindowCenter().y * 3, getWindowCenter().y * 3),
		800.0f
		));

		_scale = Anim<Vec3f>(Vec3f::one());
		_state = State::WAIT;
}

void Name::update()
{
	wait();
}

void Name::draw()
{
	gl::pushModelView();

	// ローカル
	gl::translate(Vec3f(-_pos.value().x, _pos.value().y, _pos.value().z));
	gl::rotate(Vec3f(0, 0, 180));
	gl::scale(_scale);

	gl::drawStringCentered(
		ci::toUtf8((char16_t*)_name.c_str()),
		Vec2f::zero(), 
		ColorA::white(),
		getFont()
		);
	gl::popModelView();
}

void Name::startDirection(const float duration_time)
{
	_state = State::DRAIN;

	const float time = 1.0f + duration_time;

	// amplitude 振幅
	// period 1回にゆれる時間

	timeline().apply(
		&_pos, 
		_pos.value(), 
		Vec3f(0, 0,900),
		time,
		EaseInElastic(10.0f, 5.0f)
		);

	timeline().apply(
		&_scale, 
		_scale.value(), 
		Vec3f::zero(), 
		time,
		easeInExpo
		);

}

void Name::setAnnounce()
{
	is_delete = true;
	_pos.stop();
	_scale.stop();
	timeline().apply(&_pos, _pos.value(), Vec3f(0, 0, 600), 4.0f,EaseOutBounce());
	timeline().apply(&_scale, _scale.value(), Vec3f::one(), 4.0f, EaseOutBounce());
	_state = State::ANNOUNCE;
}