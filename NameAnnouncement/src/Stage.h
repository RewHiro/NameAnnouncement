#pragma once
#include "Scene.h"

#include "Name.h"
#include "StarManager.h"

#include "cinder/gl/gl.h"
#include "cinder\Camera.h"
#include "cinder\gl\Light.h"
#include "cinder\gl\Texture.h"
#include "cinder\ImageIo.h"
#include "cinder\app\App.h"

#include <memory>



class Stage : public Scene
{

	ci::CameraPersp _camera;
	std::vector<std::shared_ptr<Name>>_namelist;

	std::unique_ptr<ci::gl::Light>_light;

	ci::gl::Texture _background_texture;

	enum class State
	{
		WAIT,
		DRAIN,
		ANNOUNCEMENT
	};

	StarManager _star_manager;

	State _state = State::WAIT;

	int _num = 0;
	int _count = 0;
	int _index = 0;

	void drain();
	void wait();
	void announce();
	void transiton();

public:

	Stage();
	SceneType update();
	void draw();
	void resize();

	static std::string& getClassName()
	{
		static std::string instance;
		return instance;
	}
};