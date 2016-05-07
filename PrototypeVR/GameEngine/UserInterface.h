#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

typedef struct
{
	int joystickID;
	int numberOfButtons;
	float AxesPositions[12];

}JOYSTICK;

typedef struct
{
	int numberOfButtons;
	float posX;
	float posY;
	bool  inWindow;
}MOUSE;

static JOYSTICK joystick1;

void initInputInterface(GLFWwindow* window, bool &joystickActive);
void ChangeSize(int w, int h);
void SpecialKeys(int key, int inX, int inY);
void KeyAction(GLFWwindow* window, int key, int action);
void MouseAction(GLFWwindow* window,int  button,int  action);
void CursorPosition(GLFWwindow* window, float x_pos, float y_pos);
void CursorInView(GLFWwindow* window, int enter);
void KeyPressed(unsigned char c, int x, int y);
void SpecialMouse(int button, int state, int _x, int _y);


void OnRotate(float pX, float pY);
void OnRotateViewer(float pX, float pY);
void OnOrbit(float pX, float pY);
void OnPan(float pX, float pY);

class MovementData
{
public:
	MovementData();
	void leftOn();
	void leftOff();
	void rightOn();
	void rightOff();
	void middleOn();
	void middleOff();
	bool isLeftDown();
	bool isRightDown();
	bool isMiddleDown();
	bool isLeftAndRightDown();
	float cMovementX(float iX);
	float cMovementY(float iY);
	void updateLastXY(float iX, float iY);


private:
	bool leftDown;
	bool rightDown;
	bool middleDown;
	float lastX;
	float lastY;
};

#endif