#include "StarManager.h"
#include "cinder\app\App.h"

using namespace ci;
using namespace ci::app;

StarManager::StarManager():
STAR_NUM(200)
{

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
	StarManager::getTexture() = loadImage(loadAsset("particle.png"));
	for (auto& star : stars)
	{
		star->setAnnouncement();
	}
}

void StarManager::reset()
{
	StarManager::getTexture() = loadImage(loadAsset("corona.png"));
	for (auto& star : stars)
	{
		star->reset();
	}

}