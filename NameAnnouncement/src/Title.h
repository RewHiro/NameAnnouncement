#pragma once
#include "Scene.h"

#include "cinder\Font.h"

class Title : public Scene
{
	ci::Font _font;
	std::vector<std::string>_class_list;
	int _select_count;
	int _index;
public:
	Title();
	SceneType update();
	void draw();
	void resize();
};