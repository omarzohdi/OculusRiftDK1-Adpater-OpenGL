#include "VR_Avatar.h"

VR_Avatar::VR_Avatar(void)
{
	vr_camera = new VR_Camera();
}

VR_Avatar::~VR_Avatar(void)
{
	delete vr_camera;
}

void VR_Avatar::InitializeCamera(const Viewport & Vp, float FoV,const Vect & Up,const Vect & Pos,const Vect & Lookat)
{
	vr_camera->InitializeVRCamera(Vp, FoV, Up, Pos, Lookat);
}

void VR_Avatar::InitializeEyes()
{
	Viewport viewport(vr_camera->getViewport());

	this->vr_left_eye = new VR_Eye();
	this->vr_left_eye->InitializeEye(Vect(0.0f,0.0f,0.0f),Vect(0.0f,0.0f,0.0f), Viewport(viewport.x,viewport.y, viewport.width, viewport.height), (EyePosition)0);
	this->vr_left_eye->setAvatar(this);

	float IPD = VRManager::getInstance().getAdaptor()->getIPD();

	this->vr_right_eye = new VR_Eye();
	this->vr_right_eye->InitializeEye(Vect(IPD,0.0f,0.0f),Vect(IPD,0.0f,0.0f),Viewport(viewport.width,viewport.y, viewport.width, viewport.height),(EyePosition)1);
	this->vr_right_eye->setAvatar(this);

	this->currentEye = this->vr_left_eye;
}

void VR_Avatar::Update()
{

}

void VR_Avatar::RenderEye(GraphicsObjectManager * gom)
{
	vr_camera->updateCamera( this->currentEye->getPosOffset(), this->currentEye->getProjectionMatrix(), this->currentEye->getViewMatrix());
	this->currentEye->Render(gom);
}

void VR_Avatar::RenderLeft(GraphicsObjectManager * gom)
{
	this->currentEye = this->vr_left_eye;
	//VRManager::getInstance().UpdateSensorRenderData();
	RenderEye(gom);
}

void VR_Avatar::RenderRight(GraphicsObjectManager * gom)
{
	this->currentEye = this->vr_right_eye;
//	VRManager::getInstance().UpdateSensorRenderData();
	RenderEye(gom);
}

VR_Eye * VR_Avatar::getLeftEye()
{
	return this->vr_left_eye;
}
VR_Eye * VR_Avatar::getRightEye()
{
	return this->vr_right_eye;
}

Matrix & VR_Avatar::getCameraProjectionMatrixwithOffset(EyePosition eyeposition)
{
	switch(eyeposition)
	{
		case LEFT_EYE:
			return vr_camera->getProjMatrix(vr_left_eye->getPosOffset(),vr_left_eye->getLookOffset());	
		case RIGHT_EYE:
			return vr_camera->getProjMatrix(vr_right_eye->getPosOffset(),vr_right_eye->getLookOffset());	
		default:
			return vr_camera->getProjMatrix(vr_left_eye->getPosOffset(),vr_left_eye->getLookOffset());	
	}
}	
Matrix & VR_Avatar::getCameraViewMatrixwithOffset(EyePosition eyeposition)
{
	switch(eyeposition)
	{
		case LEFT_EYE:
			return vr_camera->getViewMatrix(vr_left_eye->getPosOffset(),vr_left_eye->getLookOffset());	
		case RIGHT_EYE:
			return vr_camera->getViewMatrix(vr_right_eye->getPosOffset(),vr_right_eye->getLookOffset());	
		default:
			return vr_camera->getViewMatrix(vr_left_eye->getPosOffset(),vr_left_eye->getLookOffset());	
	}
}

Viewport & VR_Avatar::getEyeViewport(EyePosition eyePosition)
{
	switch(eyePosition)
	{
		case LEFT_EYE:
			return vr_left_eye->getViewport();
		case RIGHT_EYE:
			return vr_right_eye->getViewport();
		default:
			return vr_left_eye->getViewport();
	}
}