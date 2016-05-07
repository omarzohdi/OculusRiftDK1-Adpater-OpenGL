#pragma once


////Rules////
////The Projected Flow of the Application.... FOLLOW IT BY THE LAW! NEVER COMPROMISE!
/*
//Rendering
->The Adaptor doesn't take care of the camera! the Engine does.
->The Adaptor has calls to set the drawing to each eye ( StarLeftEyeRender() etc..)
->The Adaptor Applies Post process rendering
->The Adaptor Returns the new OPENGL Buffers.

//Movement
->The Adaptor Calculates the Oculus Matrix.
->The Adaptor Returns the Oculus Matrix to the Engine
->The Engine is Responsible for updating it's own Matricies.

//Calibraton
->The Calibration tool and the Config Load are a decoupled systemn.
->Works with the Adaptor but it's not part of it.
->It can load the config of any VR HMD
->It can Calibrate Buffers (changes the value of the Matricies returned by the Adaptor)

*/

class PrototypeVR
{
public:
	PrototypeVR(void);
	~PrototypeVR(void);
};

