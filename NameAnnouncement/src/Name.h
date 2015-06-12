#pragma once

#include "cinder\Font.h"
#include "cinder\Timeline.h"
#include "boost\noncopyable.hpp"
#include "cinder\app\App.h"

class Name : private boost::noncopyable
{

	std::wstring _name;
	cinder::Anim<ci::Vec3f>_pos;
	cinder::Anim<ci::Vec3f>_scale;

public:

	static ci::Font& getFont()
	{
		static ci::Font instance;
		return instance;
	}

	static void setFont(const std::string& font_name, const float font_size)
	{
		getFont() = ci::Font(ci::app::loadAsset(font_name), font_size);
	}

	Name(const std::wstring& name);
	void update();
	void draw();
};