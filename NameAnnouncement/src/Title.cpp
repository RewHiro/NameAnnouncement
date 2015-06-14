#include "Title.h"
#include <filesystem>

#include "cinder\gl\gl.h"
#include "cinder\app\App.h"
#include "cinder\Unicode.h"

#include "Key.h"
#include "Utility.h"
#include "Stage.h"

using namespace ci;
using namespace ci::app;
using namespace input;


Title::Title():
_select_count(0),
_index(0)
{
	namespace sys = std::tr2::sys;
	sys::path p("../assets/name_list");
	std::for_each(sys::directory_iterator(p), sys::directory_iterator(),
		[this](const sys::path& p)
	{
		auto filename = p.filename();
		auto class_name = filename.substr(0, filename.find("."));
		_class_list.emplace_back(class_name);
	});

	_font = Font("Meiryo", 100);
	setlocale(LC_CTYPE, "");
	gl::disable(GL_LIGHTING);
	gl::disable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_NORMALIZE);
}

SceneType Title::update()
{
	const auto& key = Key::getInstance();

	if(key.isPush(KeyEvent::KEY_UP))
	{
		--_select_count;
		auto delta = static_cast<int>(_class_list.size());
		_index = std::abs((_select_count + delta) % delta);
	}
	else if (key.isPush(KeyEvent::KEY_DOWN))
	{
		++_select_count;
		_index = std::abs(_select_count % static_cast<int>(_class_list.size()));
	}

	if(key.isPush(KeyEvent::KEY_RETURN))
	{
		type = SceneType::STAGE;
		Stage::getClassName() = _class_list[_index];
	}



	return type;
}
void Title::draw()
{

	std::wstring choice_str = L"どのクラスを選びますか？";
	gl::pushModelView();
	gl::translate(getWindowCenter().x, getWindowCenter().y *0.5);
	gl::scale(0.5f, 0.5f);
	gl::drawStringCentered(
		toUtf8((char16_t*)choice_str.c_str()),
		Vec2f::zero(),
		ColorA::white(),
		_font
		);
	gl::popModelView();

	gl::pushModelView();
	gl::translate(getWindowCenter().x, getWindowCenter().y + getWindowCenter().y * 0.25f);
	gl::scale(0.5f, 0.5f);

	gl::drawStringCentered(
		_class_list[_index],
		Vec2f::zero(),
		ColorA::white(),
		_font
		);
	gl::popModelView();
}

void Title::resize()
{

}
