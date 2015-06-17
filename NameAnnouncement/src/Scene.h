#pragma once
#include "boost\noncopyable.hpp"


enum class SceneType
{
	TITLE,
	STAGE,
};

class Scene : private boost::noncopyable
{

protected:
	SceneType type;

public:
	Scene() = default;
	virtual ~Scene();
	virtual SceneType update() = 0;
	virtual void draw() = 0;
	virtual void resize(){}
	virtual void shutdown(){}
	void setSceneType(const SceneType type){ this->type = type; }
};