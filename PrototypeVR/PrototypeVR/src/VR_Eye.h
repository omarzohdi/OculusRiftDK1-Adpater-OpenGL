#ifndef VREYE_H
#define VREYE_H

#include "AzulCore.h"
#include "VRStructs.h"


class VR_Avatar;

class VR_Eye
{

public:
	VR_Eye(void);
	~VR_Eye(void);

	void InitializeEye(const Vect & posoffset, const Vect & lookoffset, const Viewport & viewport, EyePosition eyeposition);
	void Render(GraphicsObjectManager *);

	void setAvatar(VR_Avatar *);
	Viewport & getViewport();

	Vect getPosOffset();
	Vect getLookOffset();

	void setHorCalibOffset(float);
	void setVerCalibOffset(float);

	Matrix & getoffsetViewMatrix();
	Matrix & getoffsetPorjectionMatrix();

	Matrix & VR_Eye::getViewMatrix();
	Matrix & VR_Eye::getProjectionMatrix();

	Vect getDir();
	Vect getUp();

	///////////////////Temp///////////////
	void setViewMatrix(const Matrix&);

	void getDistortionRenderData(float *, float *, float *, float&, float&);

private:

	VR_Avatar * vr_avatar;

	EyePosition eyepositon;	
	Vect posoffset;
	Vect lookoffset;
	Vect caliboffset;

	Matrix ProjectionMatrix;
	Matrix ViewMatrix;

	Viewport viewport;

	float DistortionK[4];
	float LensOffset[2];
	float renderScale;
	float DisAspect;
};

#endif