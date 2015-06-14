#include "StarManager.h"

using namespace ci;

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