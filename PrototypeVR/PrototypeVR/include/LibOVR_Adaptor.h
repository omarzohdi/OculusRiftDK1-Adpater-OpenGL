#ifndef OVRADAPT_H
#define OVRADAPT_H

#include "OVR_CAPI_0_5_0.h"
#include "OVR_CAPI_GL.h"
#include "OVR.h"

#include "Kernel\OVR_System.h"

#include "VRStructs.h"

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))


class LibOVR_Adaptor
{
public:
	LibOVR_Adaptor(void);
	~LibOVR_Adaptor(void);


	void Initialize(void);
	/*
	void	getCurrentEyeProjectioViewMatrix(Matrix& Projection, Matrix& View, Viewport& Vp, EyePosition Eye);
	void	getCurrentEyeDistortionData(float *, float *, float&, float&DisAspect, EyePosition);
	void	getViewportPrespectiveData(float &fov, float &AspectRatio);	

	float	getDistortionScale();
	int		getHorizonatalResolution();
	int		getVerticalResolution();
	float	getIPD();
	double  getVRTime();
	void	getVRYawPitchRoll(float&, float &, float &);
	void	setVRYawPitchRoll(float , float , float );
	void 	setVRLastUpdate( double );
	bool	isSensorInit();
	void	setVRLastYAW(float );
	float	getVRLastYAW();

	void CalculateYawPitchRoll();
	Matrix getViewMatrixAfterMovement();
	*/

private:

	ovrHmd hmd;

	/*
	float IPD_Unit_Conversion;
	float h;
	float d;
	int HorRez;
	int VerRez;

	OVR::Ptr<OVR::DeviceManager> pManager;
	OVR::Ptr<OVR::HMDDevice> pHMD;
	
	OVR::HMDInfo HMDinfo;
	OVR::Util::Render::StereoConfig stereo;
	
	OVR::Ptr<OVR::SensorDevice> pSensor;
	OVR::SensorFusion * pSFusion;

	OVR::Vector3f       EyePos;
    float               EyeYaw;         // Rotation around Y, CCW positive when looking at RHS (X,Z) plane.
    float               EyePitch;       // Pitch. If sensor is plugged in, only read from sensor.
    float               EyeRoll;        // Roll, only accessible from Sensor.
    float               LastSensorYaw;  // Stores previous Yaw value from to support computing delta.
	unsigned __int64    StartupTicks;
	double              LastUpdate; ///Take out th entire timer thing, don't think it's even needed.
	*/
};

#endif