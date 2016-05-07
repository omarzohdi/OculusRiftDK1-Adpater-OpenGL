#ifndef VRAVATAR_H
#define VRAVATAR_H

#include "VR_Camera.h"
#include "VR_Eye.h"
#include "VRManager.h"

class VR_Avatar
{
public:

	VR_Avatar(void);
	~VR_Avatar(void);

	Matrix & getCameraViewMatrixwithOffset(EyePosition eyeposition);
	Matrix & getCameraProjectionMatrixwithOffset(EyePosition eyeposition);

//	Matrix & getCurrentEyeViewMatrix();
//	Matrix & getCurrentEyeProjMatrix();

	VR_Eye * getLeftEye();
	VR_Eye * getRightEye();

	Viewport & getEyeViewport(EyePosition eyePosition);

	void InitializeEyes();
	void InitializeCamera(const Viewport & Vp, float FoV,const Vect & Up, const Vect & Pos,const Vect & Lookat);
	void Update();

	void RenderLeft(GraphicsObjectManager * );
	void RenderRight(GraphicsObjectManager *);
	void RenderEye(GraphicsObjectManager *);
	VR_Eye * currentEye;

private:
	
	//Starting distance between both eyes!!
	int IPD; // used later with calibration tool and multiple avatars 

	VR_Eye * vr_left_eye;
	VR_Eye * vr_right_eye;

	VR_Camera * vr_camera;
};

#endif;