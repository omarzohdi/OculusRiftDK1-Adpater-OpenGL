#ifndef VRMANAGER_H
#define VRMANAGER_H

#define AVATARS 1

#include "VR_Camera.h"
#include "VR_Eye.h"
#include "VR_Avatar.h"
#include "LibOVR_Adaptor.h"

class VR_Calibrator;
class VR_Canvas;

class VRManager
{

public:
	static VRManager & getInstance();
	
	~VRManager(void);
	void StartAdaptor();
	void InitializeManager(const Vect & Up,const Vect & Pos,const Vect & Lookat);
	void InitializeAvatars();
	void InitializeCamera(VR_Avatar & Avatar,const Viewport & Vp, float FoV,const Vect & Up, const Vect & Pos, const Vect & Lookat);
	void InitializeEyes();
	void InitializeCanvasPanes();
	void InitializeCanvas();
	void InitializeCalibrationTool();
	void InitializeShader();


	void update();
	void sensorUpdate();
	void UpdateSensorRenderData();
	void render(GraphicsObjectManager *,Camera *);
	

	void getScreenResolution(int &HorSize, int &VerSize);
	void getViewportSize(int &width, int &height);
	GLuint getShaderProgram();
	LibOVR_Adaptor * getAdaptor(void);
	VR_Canvas & getCurrentCanvas();

	Matrix & VRManager::getCurrentEyeViewMatrix( );
	Matrix & VRManager::getCurrentEyeProjMatrix( );

	void ActivateCalibrationMode();
	void DeactivateCalibrationMode();

	//VR_Avatar * currentAvatar;
	VR_Canvas * CurrentCanvas;
	
	void SwitchCalibrationMode(int );
	void Calibrate(VR_Avatar *);
	bool isCalibrationOn();
	void setCalibrationAmount(float);

private:

	VRManager(void);

	VR_Calibrator * Calibrator;
	GLuint ShaderProgram;

	/////Make these into linked lists///
	VR_Avatar * AvatarsList;
	VR_Eye * EyeList;
	VR_Camera * CameraList;
	VR_Canvas * CanvasList;
	////////////////////////////////////

	LibOVR_Adaptor * Adaptor;
};

#endif
