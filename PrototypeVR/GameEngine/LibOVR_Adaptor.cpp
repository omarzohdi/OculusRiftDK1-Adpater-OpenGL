#include "LibOVR_Adaptor.h"



LibOVR_Adaptor::LibOVR_Adaptor(void)
	:IPD_Unit_Conversion( 512.5 )
{
	pSFusion = 0;
	OVR::System::Init(OVR::Log::ConfigureDefaultLog(OVR::LogMask_All));

	////Get EDID of Device and get required Info
	pManager = *OVR::DeviceManager::Create();
	pHMD = *pManager->EnumerateDevices<OVR::HMDDevice>().CreateDevice();

#if VRDEBUG
	//////////////////Checking Info From Device///////////////////
	//////////////////////////////////////////////////////////////
	char * DevName;
	
	float IPD;
	float Distortion[4];
	if (pHMD)
	{
		if (pHMD->GetDeviceInfo(&HMDinfo))
		{
			DevName = HMDinfo.DisplayDeviceName;
			IPD = HMDinfo.InterpupillaryDistance;
			Distortion[0] = HMDinfo.DistortionK[0];
			Distortion[1] = HMDinfo.DistortionK[1];
			Distortion[2] = HMDinfo.DistortionK[2];
			Distortion[3] = HMDinfo.DistortionK[3];
		}
	}
	///////////////////////////////////////////////////////////////
#endif
	
	if (pHMD) // if the Device is Connected no need for MACRO
	{
		StartupTicks = OVR::Timer::GetTicks();
		EyePos = OVR::Vector3f(0.0f, 0.0f, 1.0f);
		EyeYaw = OVR::Math<float>::Pi;
		EyePitch = 0;
		EyeRoll = 0;
		LastSensorYaw = 0;

		pHMD->GetDeviceInfo(&HMDinfo);

		h = 4 * (( (HMDinfo.HScreenSize /4) - (HMDinfo.LensSeparationDistance/2)) ) /HMDinfo.HScreenSize;
		d = (HMDinfo.InterpupillaryDistance /2) * IPD_Unit_Conversion;

		HorRez = HMDinfo.HResolution;
		VerRez = HMDinfo.VResolution;

		/////Get Sensor Data
		pSensor = *pHMD->GetSensor();
		//OVR::SensorFusion SFusion;
		//pSFusion = &SFusion;
		
		if(pSensor)
		{
			pSFusion = new OVR::SensorFusion();
			pSFusion->AttachToSensor(pSensor);
			//pSFusion->SetDelegateMessageHandler(this);
			pSFusion->SetPredictionEnabled(true);
	//		this->getViewMatrixAfterMovement(Vect(0.0f,1.0f,0.0f), Vect(0.0f, 0.0f, 5.0f));
			
		}

	}
	else
	{
		//Default Oculus DK1 HMD Info.
		h = (float) ( 4 * (( (0.149759993 / 4) - (0.0635000020 /2)) ) / 0.149759993);
		d = (float) ( (0.0640000030 / 2 ) * IPD_Unit_Conversion);

		HorRez = 1280;
		VerRez = 800;

		pSensor = *pManager->EnumerateDevices<OVR::SensorDevice>().CreateDevice();
	}


	////////////////////Stereo Confing Rendering/////////////////////////
	
	stereo.SetFullViewport(OVR::Util::Render::Viewport(0,0, HMDinfo.HResolution,HMDinfo.VResolution));
	stereo.SetStereoMode(OVR::Util::Render::Stereo_LeftRight_Multipass);
	stereo.SetHMDInfo(HMDinfo);
	stereo.SetDistortionFitPointVP(-1.0f,0.0f);

	/////////////////////////////////////////////////////////////////////	

}


////Get Left Projection and View Matrix

void LibOVR_Adaptor::getCurrentEyeDistortionData(float * distortionK, float * lensoffset, float&scale, float&DisAspect, EyePosition EyePos)
{
	if (pHMD)
	{
		OVR::Util::Render::DistortionConfig Distortion;
		if (EyePos == EyePosition::LEFT_EYE)
		{
			Distortion = stereo.GetDistortionConfig();

			distortionK[0] = Distortion.K[0];
			distortionK[1] = Distortion.K[1];
			distortionK[2] = Distortion.K[2];
			distortionK[3] = Distortion.K[3];

			scale = stereo.GetDistortionScale();
			lensoffset[0] = Distortion.XCenterOffset;
			lensoffset[1] = Distortion.YCenterOffset;
		}

		if (EyePos == EyePosition::RIGHT_EYE)
		{
			Distortion = stereo.GetDistortionConfig();
		
			distortionK[0] = Distortion.K[0];
			distortionK[1] = Distortion.K[1];
			distortionK[2] = Distortion.K[2];
			distortionK[3] = Distortion.K[3];

			scale = stereo.GetDistortionScale();
			lensoffset[0] = Distortion.XCenterOffset;
			lensoffset[1] = Distortion.YCenterOffset;
		}

		DisAspect = stereo.GetAspect();
	}	
}

void LibOVR_Adaptor::getCurrentEyeProjectioViewMatrix(Matrix& Projection, Matrix& View, Viewport& Vp, EyePosition EyePos)
{
	if (pHMD)
	{
		OVR::Matrix4f Projection;
		OVR::Matrix4f ViewAdjust;
		OVR::Util::Render::StereoEyeParams Eye;

		stereo.SetStereoMode(OVR::Util::Render::Stereo_LeftRight_Multipass);

		if (EyePos == EyePosition::LEFT_EYE)
		{
			Eye = stereo.GetEyeRenderParams(OVR::Util::Render::StereoEye_Left);
		}

		if (EyePos == EyePosition::RIGHT_EYE)
		{
			Eye = stereo.GetEyeRenderParams(OVR::Util::Render::StereoEye_Right);
		}
		
		OVR::Matrix4f Proj = Eye.Projection;
		OVR::Matrix4f View = Eye.ViewAdjust;

		Vp.SetViewport(Eye.VP.x,Eye.VP.y,Eye.VP.w,Eye.VP.h);
	}
	else
	{
		Vp.SetViewport(0,0,HorRez/2, VerRez);
	}


	Projection.set(ROW_3, Vect( this->h, 0.0f,0.0f,1.0f));
	View.set(ROW_3, Vect(this->d, 0.0f,0.0f,1.0f));
}

void LibOVR_Adaptor::getViewportPrespectiveData(float &fov, float &AspectRatio)
{

	if (pHMD)
	{
		fov = stereo.GetYFOVDegrees();
		AspectRatio = stereo.GetAspect(); ////float(HMDinfo.HResolution/2)/ float(HMDinfo.VResolution);
	}
	else
	{

		AspectRatio = HorRez * 0.5f / VerRez;
		fov = 2.0f * atan( VerRez / 2.0f) / 0.0410000011f ; 
	}

}

float LibOVR_Adaptor::getIPD()
{
	return d;
}

float LibOVR_Adaptor::getDistortionScale()
{
	if (pHMD)
	{
		return stereo.GetDistortionScale();
	}
	
	return 7; // Random number change later//

}

int LibOVR_Adaptor::getHorizonatalResolution()
{
	return HorRez;
}

int LibOVR_Adaptor::getVerticalResolution()
{
	return VerRez;
}

LibOVR_Adaptor::~LibOVR_Adaptor(void)
{	
	pManager.Clear();
	pSensor.Clear();
	pHMD.Clear();
	OVR::System::Destroy();
}

double LibOVR_Adaptor::getVRTime()
{
    return (OVR::Timer::GetTicks() - StartupTicks) * (1.0 / (double)OVR::Timer::MksPerSecond);
}

void LibOVR_Adaptor::setVRLastUpdate(double currentTime)
{
	LastUpdate = currentTime;
}

void LibOVR_Adaptor::getVRYawPitchRoll(float &Yaw, float &Pitch, float &Roll)
{
	Yaw = this->EyeYaw;
	Pitch = this->EyePitch;
	Roll = this->EyeRoll;
}

bool LibOVR_Adaptor::isSensorInit()
{
	if (this->pSensor)
		return true;
	else
		return false;
}

void LibOVR_Adaptor::setVRYawPitchRoll(float Yaw, float Pitch, float Roll)
{
	this->EyeYaw = Yaw;
	this->EyePitch = Pitch;
	this->EyeRoll = Roll;
}
void LibOVR_Adaptor::setVRLastYAW(float lastyaw)
{
	this->LastSensorYaw = lastyaw;
}
float LibOVR_Adaptor::getVRLastYAW()
{
	return this->LastSensorYaw;
}

void LibOVR_Adaptor::CalculateYawPitchRoll()
{
	///Move this to Adaptor, NO OVR reference in MAnager only in Adaptor.
	if (this->pSFusion && this->pSensor)
	{
		float    yaw = 0.0f;
		
		OVR::Quatf hmdOrient = this->pSFusion->GetPredictedOrientation();
        hmdOrient.GetEulerAngles<OVR::Axis_Y, OVR::Axis_X, OVR::Axis_Z>(&yaw, &this->EyePitch, &this->EyeRoll);

		this->EyeYaw += (yaw - this->LastSensorYaw);
		this->LastSensorYaw  = yaw;    
	}
}

Matrix LibOVR_Adaptor::getViewMatrixAfterMovement()
{
	//float headBaseToEyeHeight		= 0.15f;  // Vertical height of eye from base of head
	//float headBaseToEyeProtrusion	= 0.09f;  // Distance forward of eye from base of head

	//OVR::Matrix4f rollPitchYaw		= OVR::Matrix4f::RotationY(EyeYaw) * OVR::Matrix4f::RotationX(EyePitch) * OVR::Matrix4f::RotationZ(EyeRoll);
	//OVR::Vector3f up				= rollPitchYaw.Transform( OVR::Vector3f(0, Up[y], Up[z]));
	//OVR::Vector3f forward			= rollPitchYaw.Transform( OVR::Vector3f(Dir[x], Dir[y], Dir[z]));

	OVR::Vector3f eyeCenterInHeadFrame(0.0f, 0.15f, -0.09f);
	OVR::Vector3f shiftedEyePos = this->EyePos + eyeCenterInHeadFrame;

	EyePos.y -= eyeCenterInHeadFrame.y; // Bring the head back down to original height

	//OVR::Matrix4f View = OVR::Matrix4f::LookAtRH(shiftedEyePos, shiftedEyePos + forward, up); 

	Matrix RotX( ROT_X, this->EyePitch  * -1);
	Matrix RotY( ROT_Y, (this->EyeYaw + 180) * -1 );
	Matrix RotZ( ROT_Z, this->EyeRoll * -1  );
	Matrix Trans( TRANS, this->EyePos.x, this->EyePos.y, this->EyePos.z);

	
	return (RotY * RotX * RotZ);// * Trans);
}