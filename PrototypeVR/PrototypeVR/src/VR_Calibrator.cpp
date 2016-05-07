#include "VR_Calibrator.h"
#include <stdio.h>

VR_Calibrator::VR_Calibrator(void)
{
	this->IsCalibrating = false;
	this->CurrentMode = Calibration_Mode::LEFTHORSHIFT;

	this->CalibrationAmount = 0.0f;
}

VR_Calibrator::~VR_Calibrator(void)
{
}

void VR_Calibrator::setCalibrating(bool State)
{
	IsCalibrating = State;
}

bool VR_Calibrator::isCalibratorOn()
{
	return IsCalibrating;
}

void VR_Calibrator::Calibrate(VR_Avatar * inAvatar)
{
	switch (CurrentMode)
	{
	case LEFTHORSHIFT:
		inAvatar->getLeftEye()->setHorCalibOffset(CalibrationAmount);		
		break;
	case RIGHTHORSHIFT:
		inAvatar->getRightEye()->setHorCalibOffset(CalibrationAmount);
		break;
	case LEFTVERSHIFT:
		inAvatar->getLeftEye()->setVerCalibOffset(CalibrationAmount);
		break;
	case RIGHTVERSHIFT:
		inAvatar->getRightEye()->setVerCalibOffset(CalibrationAmount);
		break;
	case FULLVERSHIFT:
		inAvatar->getLeftEye()->setVerCalibOffset(CalibrationAmount);
		inAvatar->getRightEye()->setVerCalibOffset(CalibrationAmount);
		break;
	case FULLHORSHIFT:
		inAvatar->getLeftEye()->setHorCalibOffset(CalibrationAmount);
		inAvatar->getRightEye()->setHorCalibOffset(CalibrationAmount);
		break;
	default:
		break;
	}

//	printf("LH: %f, LV: %f,RH: %f, RV: %f\n", LeftHorOffset, LeftVerOffset, RightHorOffset, RightVerOffset);
	CalibrationAmount = 0.0f;
	//CurrentMode = Calibration_Mode::IDLE;
}

void VR_Calibrator::setCalibrationAmount(float inAmount)
{
	this->CalibrationAmount = inAmount;
}

void VR_Calibrator::SwitchCalibrationMode( int Key)
{
	this->CurrentMode = (Calibration_Mode)( Key);
	printf("CURRENT MODE: %d\n", this->CurrentMode);
}
