#include "VR_Eye.h"
#include "VR_Avatar.h"

VR_Eye::VR_Eye(void)
{
}

VR_Eye::~VR_Eye(void)
{
}

void VR_Eye::setAvatar(VR_Avatar * avatar)
{
	this->vr_avatar = avatar;
}
void VR_Eye::InitializeEye(const Vect & posoffset, const Vect & lookoffset, const Viewport & viewport, EyePosition eyeposition)
{
	this->posoffset = posoffset;
	this->lookoffset = lookoffset;
	this->eyepositon = eyeposition;
	this->viewport = viewport;
	this->caliboffset.set(Vect(0.0f,0.0f,0.0f,0.0f));
	this->ProjectionMatrix.set(IDENTITY);
	this->ViewMatrix.set(IDENTITY);
	
 	VRManager::getInstance().getAdaptor()->getCurrentEyeProjectioViewMatrix(this->ProjectionMatrix,this->ViewMatrix, this->viewport, this->eyepositon);
	VRManager::getInstance().getAdaptor()->getCurrentEyeDistortionData(this->DistortionK,this->LensOffset, this->renderScale, this->DisAspect, this->eyepositon );
}

Matrix & VR_Eye::getViewMatrix()
{
	return this->ViewMatrix;
}
Matrix & VR_Eye::getProjectionMatrix()
{
	return this->ProjectionMatrix;
}

Matrix & VR_Eye::getoffsetViewMatrix()
{	
	return vr_avatar->getCameraViewMatrixwithOffset(eyepositon);
}

Matrix & VR_Eye::getoffsetPorjectionMatrix()
{
	return vr_avatar->getCameraProjectionMatrixwithOffset(eyepositon);
}

Vect VR_Eye::getPosOffset()
{
	return (this->posoffset + this->caliboffset);
}
Vect VR_Eye::getLookOffset()
{
	return (this->lookoffset + this->caliboffset);
}

void VR_Eye::Render(GraphicsObjectManager * gom)
{
	gom->drawObjects();
}

Viewport & VR_Eye::getViewport()
{
	return this->viewport;
}

void VR_Eye::setHorCalibOffset(float calibAmount)
{
	this->caliboffset[x] += calibAmount;

	printf("HOR -> %f \n", this->caliboffset[x]);
}
void VR_Eye::setVerCalibOffset(float calibAmount)
{
	this->caliboffset[y] += calibAmount;
}

Vect VR_Eye::getDir()
{
	return Vect(this->ViewMatrix[m2], this->ViewMatrix[m6], this->ViewMatrix[m10], 0.0f);
}

Vect VR_Eye::getUp()
{
	return Vect(this->ViewMatrix[m1], this->ViewMatrix[m5], this->ViewMatrix[m9], 0.0f);
}
void VR_Eye::setViewMatrix(const Matrix &inMatrix)
{
	this->ViewMatrix = inMatrix;
}

void VR_Eye::getDistortionRenderData(float * DistK, float * TexRange, float *LensOff, float&TexScale, float&aspect)
{
	DistK[0]  = this->DistortionK[0];
	DistK[1]  = this->DistortionK[1];
	DistK[2]  = this->DistortionK[2];
	DistK[3]  = this->DistortionK[3];

	LensOff[0] = this->LensOffset[0];
	LensOff[1] = this->LensOffset[0];
	
	TexRange[0] = (float)this->viewport.width;
	TexRange[1] = (float)this->viewport.height;

	TexScale = this->renderScale;
	aspect = this->DisAspect;
}