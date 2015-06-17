#include "StarManager.h"
#include "cinder\app\App.h"
#include "cinder\Json.h"

using namespace ci;
using namespace ci::app;

StarManager::StarManager():
STAR_NUM(200)
{
	JsonTree load(loadAsset("textures/texture.json"));
	std::string texture_root = "textures/";
	auto texture_name = load["Wait"]["FileName"].getValue<std::string>();
	getTexture() = ci::loadImage(ci::app::loadAsset(texture_root + texture_name));
}

void StarManager::setup()
{
	for (int i = 0; i < STAR_NUM;++i)
	{
		stars.emplace_back(std::make_shared<Star>());
	}
}

void StarManager::update()
{
	for(auto& star : stars)
	{
		star->update();
	}
}

void StarManager::draw()
{

	for (auto& star : stars)
	{
		star->draw();
	}

}

void StarManager::setDrain()
{
	for (auto& star : stars)
	{
		star->setDrain();
	}
}

void StarManager::setAnnnoucement()
{
	JsonTree load(loadAsset("textures/texture.json"));
	std::string texture_root = "textures/";
	auto texture_name = load["Announce"].getValue<std::string>();
	getTexture() = ci::loadImage(ci::app::loadAsset(texture_root + texture_name));
	for (auto& star : stars)
	{
		star->setAnnouncement();
	}
}

void StarManager::reset()
{
	JsonTree load(loadAsset("textures/texture.json"));
	std::string texture_root = "textures/";
	auto texture_name = load["Wait"]["FileName"].getValue<std::string>();
	getTexture() = ci::loadImage(ci::app::loadAsset(texture_root + texture_name));
	for (auto& star : stars)
	{
		star->reset();
	}

}