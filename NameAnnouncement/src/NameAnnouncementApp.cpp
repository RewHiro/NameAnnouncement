#include "cinder/app/AppNative.h"
#include "Name.h"
#include "cinder/gl/gl.h"
#include "cinder\Camera.h"
#include "cinder\Json.h"
#include <unordered_map>
#include <memory>
#include <codecvt>

//�@string��wstring�ɕϊ�����֐�
//�@@param {string} [str] - �ϊ��������}���`�o�C�g������
//�@@param {wstring} [dest] - �ϊ�����wstring
void widen(const std::string& str,std::wstring& dest)
{
	wchar_t* wcs = new wchar_t[str.length() + 1];
	mbstowcs(wcs, str.c_str(), str.length() + 1);
	dest = wcs;
	delete[]wcs;
}

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
	setlocale(LC_CTYPE, "");

	Name::setFont("naguri.ttf", 200);

	JsonTree load(loadAsset("test.json"));

	const auto& objects = load["Name"];

	for (size_t i = 0; i < objects.getNumChildren();++i)
	{
		std::wstring wstr;
		widen(objects[i].getValue<std::string>(), wstr);
		_namemap.emplace(wstr, std::make_shared<Name>(wstr));
	}

	_camera.setPerspective(90, getWindowAspectRatio(), 0.5f, 1000.0f);
	_camera.lookAt(Vec3f::zAxis() * -10, Vec3f::zero(), Vec3f::yAxis());

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

	gl::disableAlphaBlending();
}

CINDER_APP_NATIVE( NameAnnouncementApp, RendererGl )
