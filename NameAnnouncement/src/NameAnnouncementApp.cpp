#include "cinder/app/AppNative.h"
#include "Name.h"
#include "cinder/gl/gl.h"
#include "cinder\Camera.h"
#include <unordered_map>
#include <memory>


using namespace ci;
using namespace ci::app;

class NameAnnouncementApp : public AppNative {

	CameraPersp _camera;
	std::unordered_map<std::wstring, std::shared_ptr<Name>>_namemap;
  public:
	void setup()final;
	void mouseDown( MouseEvent event )final;	
	void update()final;
	void draw()final;
};

void NameAnnouncementApp::setup()
{
	_camera.setPerspective(90, getWindowAspectRatio(), 0.5f, 1000.0f);
	_camera.lookAt(Vec3f::zAxis() * -10, Vec3f::zero(), Vec3f::yAxis());

	Name::setFont("naguri.ttf", 200);
	_namemap.emplace(L"êVà‰ëÂàÍ", std::make_shared<Name>(L"êVà‰ëÂàÍ"));

}

void NameAnnouncementApp::mouseDown( MouseEvent event )
{
}

void NameAnnouncementApp::update()
{
}

void NameAnnouncementApp::draw()
{
	//gl::setMatricesWindow(getWindowSize());
	gl::enableAlphaBlending();

	// clear out the window with black
	gl::clear(Color(0.5f, 0.5f, 0.5f));


	gl::setMatrices(_camera);

	for(auto& name_object : _namemap)
	{
		name_object.second->draw();
	}
	//gl::drawStringCentered(
	//	ci::toUtf8((char16_t*)std::wstring(L"êVà‰ëÂàÍ").c_str()), 
	//	Vec2f::zero(), ColorA::white(), font
	//	);

	gl::disableAlphaBlending();
}

CINDER_APP_NATIVE( NameAnnouncementApp, RendererGl )
