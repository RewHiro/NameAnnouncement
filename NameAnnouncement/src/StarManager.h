#pragma once

#include "Star.h"

#include "cinder\gl\Texture.h"
#include "cinder\ImageIo.h"
#include "cinder\app\App.h"

#include <list>

class StarManager
{
	std::list<std::shared_ptr<Star>>stars;
	const int STAR_NUM;
public:

	StarManager();

	static ci::gl::Texture& getTexture()
	{
		static ci::gl::Texture instance;
		return instance;
	}

	void setup();
	void update();
	void draw();

	void setDrain();
	void setAnnnoucement();
	void reset();
};