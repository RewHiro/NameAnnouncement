#pragma once

#include "cinder\Font.h"
#include "cinder\Timeline.h"
#include "boost\noncopyable.hpp"
#include "cinder\app\App.h"

class Name : private boost::noncopyable
{
public:
	enum class State
	{
		WAIT,
		DRAIN,
		ANNOUNCE,
	};

private:
	bool is_delete = false;
	State _state;
	std::wstring _name;
	ci::Vec3f json_scale_;
	cinder::Anim<ci::Vec3f>_pos;
	cinder::Anim<ci::Vec3f>_scale;
	ci::Vec3f _speed;
	float angle = 0.1f;
	void wait();
	void drain();

public:

	static float& getFontScele()
	{
		static float instance;
		return instance;
	}

	static ci::Font& getFont()
	{
		static ci::Font instance;
		return instance;
	}

	static void setFont(const std::string& font_name, const float font_size)
	{
		getFont() = ci::Font(ci::app::loadAsset(font_name), font_size);
	}

	static float& getBounceTime()
	{
		static float instance;
		return instance;
	}

	Name(const std::wstring& name);
	void reset();
	void update();
	void draw();
	void startDirection(const float duration_time);
	ci::Vec3f getPos()const{ return _pos.value(); }
	State getState()const{ return _state; }
	std::wstring getName()const{ return _name; }
	void setAnnounce();
	bool isDelete()const{ return is_delete; }
	bool isFinishTween()const{ return _pos.isComplete(); }
};