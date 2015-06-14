#pragma once
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder\gl\Light.h"
#include "cinder\gl\Texture.h"
#include "cinder\ImageIo.h"

using namespace ci;
using namespace ci::app;

enum Window
{
	WIDTH = 500,
	HEIGHT = 500
};


class cStarWars
{
private:
	enum { OBJECT_MAX = 200 };

	struct Star
	{
		Vec3f start;
		Vec3f end;
		Color color;
		float angle = 0;
		float angle_plus = 2.0f;
		float move_speed = 50.0f;
	};
	Star star[OBJECT_MAX];

	int change_time = 0;


public:
	void updata(){
		change_time++;

		for (auto& stars : star){

			//�ʒu�ƐF�̕ύX
			if (change_time % 60 * 2 == 0){
				stars.start = {
					Rand::randFloat(-WIDTH / 2 * 3, WIDTH / 2 * 3),
					Rand::randFloat(-HEIGHT / 2 * 3, HEIGHT / 2 * 3),
					Rand::randFloat(0, -1000)
				};
				stars.end = {
					stars.start.x, stars.start.y,
					stars.start.z - 100
				};
				stars.color = { Color(
					Rand::randFloat(0, 1),
					Rand::randFloat(0, 1),
					Rand::randFloat(0, 1))
				};
			}

			//�ړ�
			stars.start.z += stars.move_speed;
			stars.end.z += stars.move_speed;
			if (stars.start.z > -500){
				stars.start.z = -1000;
				stars.end.z = stars.start.z - 100;
			}

			//���]
			stars.angle += stars.angle_plus;
		}

	}

	void draw(){
		for (auto& stars : star){
			//�`��
			gl::rotate(Vec3f(0, 0, stars.angle));
			gl::color(stars.color);

			gl::drawLine(stars.start, stars.end);

			gl::color(1, 1, 1);
			gl::rotate(Vec3f(0, 0, -stars.angle));
		}
	}

};

class cUniverse
{
private:

	enum { OBJECT_MAX = 150 };

	struct Star
	{
		Vec3f pos;
		Color color;
		float move_speed = 0.0f;
		float angle = 0;
		const float angle_plus = 1.0f;
		const float size = 3.0f;
	};
	Star star[OBJECT_MAX];

	int change_time = 0;

public:

	static gl::Texture& getTexture()
	{
		static gl::Texture instance = loadImage(loadAsset("sun.png"));
		return instance;
	}

	void updata(){
		change_time++;

		for (auto& stars : star){

			//�ʒu�ƐF�̕ύX
			if (change_time % 60 * 2 == 0){
				stars.pos = {
					Rand::randFloat(-getWindowCenter().x, getWindowCenter().x),
					Rand::randFloat(-getWindowCenter().y, getWindowCenter().y),
					Rand::randFloat(0.0f, 800.0f)
				};
				stars.color = { Color(
					Rand::randFloat(0, 1),
					Rand::randFloat(0, 1),
					Rand::randFloat(0, 1))
				};
			}

			//�ړ�
			stars.pos.z += stars.move_speed;
			if (stars.pos.z > 900)
				stars.pos.z = 0;

			//���]
			stars.angle += stars.angle_plus;
		}

	}

	void draw(){
		gl::enableAdditiveBlending();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		//gl::enableAlphaBlending();

		for (auto& stars : star){
			//�`��

			gl::pushModelView();
			gl::translate(stars.pos);
			//gl::rotate(Vec3f(0, 0, -stars.angle));

			gl::color(Color(0,1,0));
			gl::draw(cUniverse::getTexture(), Rectf(-10, -10, 10, 10));
			gl::popModelView();

		}
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		gl::disableAlphaBlending();

	}

};

class cYRotater
{
private:

	enum { OBJECT_MAX = 500 };
	const int POS_MAX = 1500;

	struct Line
	{
		Vec3f pos;
		Color color;
		float move_speed = 100.0f;
		float angle = 0;
		const float angle_plus = 1.0f;
		const float size = 3.0f;
	};
	Line obj[OBJECT_MAX];

	int change_time = 0;

public:

	void updata(){
		change_time++;

		for (auto& objs : obj){

			//�ʒu�ƐF�̕ύX
			if (change_time % 60 * 2 == 0){
				objs.pos = {
					Rand::randFloat(-(float)POS_MAX, (float)POS_MAX),
					Rand::randFloat(-HEIGHT / 2, HEIGHT / 2),
					Rand::randFloat(-(float)POS_MAX, (float)POS_MAX)
				};
				objs.color = { Color(
					Rand::randFloat(0, 1),
					Rand::randFloat(0, 1),
					Rand::randFloat(0, 1))
				};
			}

			//���]
			objs.angle += objs.angle_plus;
		}

	}

	void draw(){
		for (auto& objs : obj){
			//�`��
			gl::rotate(Vec3f(0, objs.angle, 0));
			gl::color(objs.color);

			gl::drawSphere(objs.pos, objs.size);

			gl::color(1, 1, 1);
			gl::rotate(Vec3f(0, 0, -objs.angle));
		}
	}

};


class BackGroundListApp : public AppNative
{
private:

	cStarWars StarWars;
	cUniverse Universe;
	cYRotater yRotater;

	enum SwitchName
	{
		STARWARS,
		UNIVERSE,
		YROTAR,
		MAX
	};
	//�X�C�b�`�̏������i�ύX�Ŕw�i�ω��j
	int background_swi = YROTAR;

public:
	void setup(){

		setWindowSize(WIDTH, HEIGHT);

		gl::pushModelView();
		gl::translate(getWindowCenter());
		gl::popModelView();

	}

	void mouseDown(MouseEvent event){}

	void update(){

		switch (background_swi){
			case STARWARS:			StarWars.updata();
				break;
			case UNIVERSE:			Universe.updata();
				break;
			case YROTAR:			yRotater.updata();
				break;
		}

	}

	void draw(){
		gl::clear(Color(0, 0, 0));

		switch (background_swi){
			case STARWARS:			StarWars.draw();
				break;
			case UNIVERSE:			Universe.draw();
				break;
			case YROTAR:			yRotater.draw();
				break;
		}

	}

};

