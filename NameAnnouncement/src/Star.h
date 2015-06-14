#pragma once
#include "cinder\Color.h"
#include "cinder\Vector.h"
#include "cinder\Timeline.h"

class Star
{
	enum class State
	{
		WAIT,
		DRAIN,
		ANNOUNCE,
	};

	State _state;
	ci::ColorA _color;
	ci::Anim<ci::Vec3f> _pos;
	ci::Anim<float> _pos_y;
	float _color_radian;

	void shake();
	void burstIn();
	void burstOut();
	void burstInOut();
	void slowmove();

public:
	Star();
	void update();
	void draw();
};