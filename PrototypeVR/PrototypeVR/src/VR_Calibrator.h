#ifndef VRCALIB_H
#define VRCALIB_H

#include "VR_Avatar.h"

enum Calibration_Mode
{
	
	IDLE = 0,

	LEFTHORSHIFT,
	RIGHTHORSHIFT,

	FULLHORSHIFT,
	
	LEFTVERSHIFT,
	RIGHTVERSHIFT,

	FULLVERSHIFT,

};

class VR_Calibrator
{
public:
	
	VR_Calibrator(void);
	~VR_Calibrator(void);

	void setCalibrating(bool);
	bool isCalibratorOn();	
	void setCalibrationAmount(float);
	void Calibrate(VR_Avatar *);
	void SwitchCalibrationMode(int );

private:

	//float LeftHorOffset;
	//float LeftVerOffset;

	//float RightHorOffset;
	//float RightVerOffset;
	
	bool IsCalibrating;
	Calibration_Mode CurrentMode;
	float CalibrationAmount;
	
};

#endif

