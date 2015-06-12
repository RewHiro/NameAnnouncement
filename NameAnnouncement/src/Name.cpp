#include "Name.h"
#include "cinder\gl\gl.h"
#include "cinder\Unicode.h"


using namespace ci;
using namespace ci::app;

Name::Name(const std::wstring& name):
_name(name),
_pos(Anim<Vec3f>(Vec3f::zero())),
_scale(Anim<Vec3f>(Vec3f::one()))
{
	timeline().apply(&_pos, _pos.value(), Vec3f::zero(), 5.0f, easeOutQuint);
	timeline().apply(&_scale, _scale.value(), Vec3f::zero(), 5.0f, easeOutQuint);
}

void Name::update()
{
}

void Name::draw()
{
	gl::pushModelView();
	gl::translate(_pos);
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