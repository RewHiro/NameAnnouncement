#pragma once
#include "Scene.h"
#include <memory>
#include "boost\noncopyable.hpp"
#include "cinder\Vector.h"

class SceneManager : private boost::noncopyable
{
	SceneType type;
	std::unique_ptr<Scene>scene;
public:
	SceneManager();
	void resize();
	void update();
	void draw();
	void shutdown();

	static ci::Vec2f& getWindowSize()
	{
		static ci::Vec2f instance;
		return instance;
	}

	static ci::Vec2f getWindowCenter()
	{
		return SceneManager::getWindowSize() * 0.5f;
	}
};