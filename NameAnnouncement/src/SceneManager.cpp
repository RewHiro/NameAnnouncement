#include "SceneManager.h"
#include "SceneFactory.h"
#include "Key.h"


SceneManager::SceneManager():
type(SceneType::TITLE),
scene(SceneFactory::getInstance().create(type))
{
	scene->setSceneType(type);
}

void SceneManager::resize()
{
	scene->resize();
}

void SceneManager::update()
{
	const auto tmp_scene_type = scene->update();
	if (type == tmp_scene_type)return;
	scene = SceneFactory::getInstance().create(tmp_scene_type);
	scene->setSceneType(tmp_scene_type);
	type = tmp_scene_type;
	input::Key::getInstance().clear();

}

void SceneManager::draw()
{
	scene->draw();
}

void SceneManager::shutdown()
{
	scene->shutdown();
}