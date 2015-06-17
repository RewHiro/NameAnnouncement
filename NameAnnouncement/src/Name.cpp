#include "Name.h"

#include "Key.h"
#include "SceneManager.h"
#include "Stage.h"

#include "cinder\gl\gl.h"
#include "cinder\Unicode.h"
#include "cinder\Rand.h"
#include "cinder\gl\TextureFont.h"
#include "cinder\Json.h"



using namespace ci;
using namespace ci::app;
using namespace input;

Name::Name(const std::wstring& name) :
_state(State::WAIT),
_name(name),
_pos(Anim<Vec3f>(Vec3f(
randFloat(-SceneManager::getWindowCenter().x * 2.5f, SceneManager::getWindowCenter().x * 2.5f),
randFloat(-SceneManager::getWindowCenter().y * 2.5f, SceneManager::getWindowCenter().y * 2.5f),
800.0f
))),
_scale(Anim<Vec3f>(Vec3f::one() * 2)),
_speed(Vec3f(
randFloat(-1, 1),
randFloat(-1, 1),
randFloat(-1.5f, 1.5f)
))
{
	_scale.value() = Vec3f::one() * getFontScele();
}

void Name::wait()
{
	if (_state != State::WAIT)return;
	_pos.value() += _speed;

	//　上限
	static const Vec3f SUPREMUM =
		Vec3f(
		SceneManager::getWindowCenter().x * 2.5f,
		SceneManager::getWindowCenter().y * 2.5f,
		900.0f
		);
	//　下限
	static const Vec3f INFIMUM =
		Vec3f(
		-SceneManager::getWindowCenter().x * 2.5f,
		-SceneManager::getWindowCenter().y * 2.5f,
		700.0f
		);

	if(_pos.value().x > SUPREMUM.x || _pos.value().x < INFIMUM.x)
	{
		_speed.x *= -1;
		_pos.value().x = std::max(_pos.value().x, INFIMUM.x);
		_pos.value().x = std::min(_pos.value().x, SUPREMUM.x);
	}
	if (_pos.value().y > SUPREMUM.y || _pos.value().y < INFIMUM.y)
	{
		_speed.y *= -1;
		_pos.value().y = std::max(_pos.value().y, INFIMUM.y);
		_pos.value().y = std::min(_pos.value().y, SUPREMUM.y);
	}
	if (_pos.value().z > SUPREMUM.z || _pos.value().z < INFIMUM.z)
	{
		_speed.z *= -1;
		_pos.value().z = std::max(_pos.value().z, INFIMUM.z);
		_pos.value().z = std::min(_pos.value().z, SUPREMUM.z);
	}
}

void Name::reset()
{
	_pos.stop();
	_scale.stop();

	_speed = Vec3f(
		randFloat(-1, 1),
		randFloat(-1, 1),
		randFloat(-1.5f, 1.5f)
		);

	_pos = Anim<Vec3f>(Vec3f(
		randFloat(-SceneManager::getWindowCenter().x * 2.5f, SceneManager::getWindowCenter().x * 2.5f),
		randFloat(-SceneManager::getWindowCenter().y * 2.5f, SceneManager::getWindowCenter().y * 2.5f),
		800.0f
		));

	_scale.value() = Vec3f::one() * getFontScele();
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

	const float time = duration_time + Stage::getDrainTime();

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
	timeline().apply(&_pos, _pos.value(), Vec3f(0, 0, 600), getBounceTime(),EaseOutBounce());
	timeline().apply(&_scale, _scale.value(), Vec3f::one() * getFontScele(), getBounceTime(), EaseOutBounce());
	_state = State::ANNOUNCE;
}