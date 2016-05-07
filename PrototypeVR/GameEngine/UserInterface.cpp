/*
#include "OpenGLWrapper.h"
#include "Azul.h"
#include "MathEngine.h"
#include "Camera.h"
#include "UserInterface.h"

#include <stdio.h>
*/
#include "AzulCore.h"


MovementData* myData;
int lastKey;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	AZUL_UNUSED( scancode );
	AZUL_UNUSED( mods );
	KeyAction( window,  key,  action);
}

static void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	AZUL_UNUSED( mods );
	MouseAction( window,  button,  action);
}

static void cursor_callback(GLFWwindow* window, double x_pos, double y_pos)
{
	CursorPosition( window,  (float)x_pos,  (float)y_pos);
}

static void cursor_active_callback(GLFWwindow* window, int enter)
{
	CursorInView( window,  enter);
}



//initialize the input callback system
void initInputInterface(GLFWwindow* window, bool &joystics)
{
	myData = new MovementData();

	AZUL_UNUSED( joystics );

	//initializing keyboard
	glfwSetKeyCallback(window, key_callback);

	//initializing mouse
	glfwSetMouseButtonCallback(window, mouse_callback);
	glfwSetScrollCallback(window, cursor_callback);
	//init mouse cursor 
	glfwSetCursorPosCallback(window, cursor_callback);
	glfwSetCursorEnterCallback(window, cursor_active_callback);


	//check for joystics and return
	int joyNum = 1;
	int count[12];

	while(glfwJoystickPresent(joyNum) == GL_TRUE)
	{
		memcpy(joystick1.AxesPositions, glfwGetJoystickAxes( GLFW_JOYSTICK_1,count ),sizeof(float)*12);
		
		joyNum++;
	}
}


void KeyAction(GLFWwindow* window, int key, int action)
{
	AZUL_UNUSED(window);
	AZUL_UNUSED(key);
	AZUL_UNUSED(action);

	/*
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_1)
	{
		VRManager::getInstance().ActivateCalibrationMode();
		VRManager::getInstance().SwitchCalibrationMode(1);
	}
	if (key == GLFW_KEY_2)
	{
		VRManager::getInstance().ActivateCalibrationMode();
		VRManager::getInstance().SwitchCalibrationMode(2);
	}
	if (key == GLFW_KEY_3)
	{
		VRManager::getInstance().ActivateCalibrationMode();
		VRManager::getInstance().SwitchCalibrationMode(3);
	}
	if (key == GLFW_KEY_4)
	{
		VRManager::getInstance().ActivateCalibrationMode();
		VRManager::getInstance().SwitchCalibrationMode(4);	
	}
	if (key == GLFW_KEY_5)
	{
		VRManager::getInstance().ActivateCalibrationMode();
		VRManager::getInstance().SwitchCalibrationMode(5);
	}
	if (key == GLFW_KEY_6)
	{
		VRManager::getInstance().ActivateCalibrationMode();
		VRManager::getInstance().SwitchCalibrationMode(6);
	}
	if (key == GLFW_KEY_0)
	{
		VRManager::getInstance().DeactivateCalibrationMode();
		VRManager::getInstance().SwitchCalibrationMode(0);
		VRManager::getInstance().setCalibrationAmount(0.0f);	
	}

	if (VRManager::getInstance().isCalibrationOn())
	{

		if (key == GLFW_KEY_RIGHT)
			VRManager::getInstance().setCalibrationAmount(0.1f);			

		if (key == GLFW_KEY_LEFT)
			VRManager::getInstance().setCalibrationAmount(-0.1f);			

		if (key == GLFW_KEY_UP)
			VRManager::getInstance().setCalibrationAmount(-0.1f);			
			
		if (key == GLFW_KEY_DOWN)
			VRManager::getInstance().setCalibrationAmount(0.1f);			
	}
	*/
}

void MouseAction(GLFWwindow* window,int  button,int  action)
{
	AZUL_UNUSED( window );
	AZUL_UNUSED( button );
	AZUL_UNUSED( action );

	if(button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS )
		myData->leftOn();	

	if(button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE )
		myData->leftOff();	

	if(button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS )
		myData->rightOn();

	if(button == GLFW_MOUSE_BUTTON_2 && action == GLFW_RELEASE )
		myData->rightOff();

	if(button == GLFW_MOUSE_BUTTON_3 && action == GLFW_PRESS )
		myData->middleOn();	

	if(button == GLFW_MOUSE_BUTTON_3 && action == GLFW_RELEASE )
		myData->middleOff();
}



void CursorPosition(GLFWwindow* window, float _x, float _y)
{
	float dx, dy;
	dx = myData->cMovementX(_x);
	dy = myData->cMovementY(_y);

	dx /=10;
	dy /=10;

	myData->updateLastXY(_x, _y);

	

	if(myData->isLeftAndRightDown())
	{
		OnOrbit(dx, dy);
	}
	else
	{
		if(myData->isLeftDown())
		{
			OnPan(dx, dy);
		}
		else if(myData->isRightDown())
		{
			OnRotate(dx, dy);
		}
	}

	if(myData->isMiddleDown())
	{
		OnRotateViewer(dx, dy);
	}


	AZUL_UNUSED( window );

}




void CursorInView(GLFWwindow* window, int enter)
{
	AZUL_UNUSED( window );
	AZUL_UNUSED( enter );
}


// Respond to mouse by moving the camera frame of reference
void SpecialMouse(int button, int state, int _x, int _y)
{
	AZUL_UNUSED( button );
	AZUL_UNUSED( state );
	AZUL_UNUSED( _y );
	AZUL_UNUSED( _x );
   
}




///////////////////////////////////////////////////////////////////////////////
// Window has changed size, or has just been created. In either case, we need
// to use the window dimensions to set the viewport and the projection matrix.
void ChangeSize(int w, int h)
{
	h;
	w;

    //pCamera = new Camera();

	//w = 1920;
	//h = 1080;
	//// set the view port
	//LeftCamera->setViewport(0, 0, w/2, h);
	//// set the perspective
	//LeftCamera->setPerspective( 35.0f, float(w)/float(h), 1.0f, 500.0f);

	//// position the camera   (up,lookat, pos)
	//LeftCamera->setOrientAndPosition( Vect(0.0f,1.0f,0.0f), Vect(0.0f,0.0f,-1.0f), Vect(0.0f,0.0f,7.0f) );
	////pCamera->setOrientAndPosition( Vect(0.0f,1.0f,0.0f), Vect(0.0f,0.0f,5.0f), Vect(0.0f,7.0f,7.0f) );

	//RightCamera->setViewport(w/2, 0, w/2, h);
	//RightCamera->setPerspective( 35.0f, float(w)/float(h), 1.0f, 500.0f);
	//RightCamera->setOrientAndPosition( Vect(0.0f,1.0f,0.0f), Vect(0.0f,0.0f,-1.0f), Vect(0.0f,0.0f,7.0f) );


}




void OnRotate(float pX, float pY)
{
	pX;
	pY;
   // Move the camera (orbit, zoom or pan) with the mouse.
   //Camera* pCamera = mainCamera;

   // position, target, up, right vector
   //Vect position, target, dir, up, right, worldUp;
   //float scale = 0.5f;

   //pCamera->getCameraUnitVector(position, target, dir, up, right, worldUp);

   //Vect cPrime(position - (pX * right * scale - pY * up * scale));

   //float distance = (position-target).mag();

   //Vect newCamTarget(target - cPrime);
   //newCamTarget.norm();

   //target = position + newCamTarget * distance;
   //
   //pCamera->setOrientAndPosition(worldUp, target, position);

};

void OnRotateViewer(float pX, float pY)
{
	pX;
	pY;
   //Camera* pCamera = mainCamera;

   // position, target, up, right vector
   //Vect position, target, dir, up, right, worldUp;
   //float scale = 0.5f;

   //pCamera->getCameraUnitVector(position, target, dir, up, right, worldUp);

   //position -= (pX * right * scale - pY * up * scale);
   //
   //pCamera->setOrientAndPosition(worldUp, target, position);
};

void OnOrbit(float pX, float pY)
{
	pX;
	pY;
   // Move the camera (orbit, zoom or pan) with the mouse.
   //Camera* pCamera = mainCamera;

   // position, target, up, right vector
   //Vect position, target, dir, up, right, worldUp;
   //float scale = 0.5f;

   //pCamera->getCameraUnitVector(position, target, dir, up, right, worldUp);


   //Vect cPrime(position - pX * right * scale);

   //float distance = (position-target).mag();

   //Vect newCamTarget(target - cPrime);
   //newCamTarget.norm();

   //target = position + newCamTarget * distance;
   //
   //position += (scale * pY* dir);
   //target += (scale * pY* dir);

   //pCamera->setOrientAndPosition(worldUp, target, position);
};

void OnPan(float pX, float pY)
{
	pX;
	pY;
   //float scale = 0.5f;
   //Vect position, target, dir, up, right, worldUp;
   ////Camera* pCamera = mainCamera;
   //pCamera->getCameraUnitVector(position, target, dir, up, right, worldUp);

   //target += pX*scale*right;
   //target[y] -= pY*scale;
   //position += pX*scale*right;
   //position[y] -= pY*scale;

   //pCamera->setOrientAndPosition(up, target, position);
};

MovementData::MovementData()
{
	this->leftDown = false;
	this->rightDown = false;
	this->middleDown = false;
	this->lastX = 0.0f;
	this->lastY = 0.0f;
};

void MovementData::leftOn()
{
	this->leftDown = true;
}
void MovementData::leftOff()
{
	this->leftDown = false;
}
void MovementData::rightOn()
{
	this->rightDown = true;
}
void MovementData::rightOff()
{
	this->rightDown = false;
}
void MovementData::middleOn()
{
	this->middleDown = true;
}
void MovementData::middleOff()
{
	this->middleDown = false;
}
bool MovementData::isLeftDown()
{
	if(leftDown == true)
		return true;
	else
		return false;
}
bool MovementData::isRightDown()
{
	if(rightDown == true)
		return true;
	else
		return false;
}
bool MovementData::isMiddleDown()
{
	if(middleDown == true)
		return true;
	else
		return false;
}
bool MovementData::isLeftAndRightDown()
{
	if(leftDown == true && rightDown == true)
		return true;
	else
		return false;
}
float MovementData::cMovementX(float iX)
{
	return iX - lastX;
}
float MovementData::cMovementY(float iY)
{
	return iY - lastY;
}
void MovementData::updateLastXY(float iX, float iY)
{
	lastX = iX;
	lastY = iY;
}