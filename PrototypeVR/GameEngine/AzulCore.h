#ifndef AZULCORE_H
#define AZULCORE_H

#define VR_ENABLED 1

#define AZUL_UNUSED(x) x

#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MAX_NAME 256

#include <GLTools.h>	// OpenGL toolkit
//#include <GL/glew.h>

//#define GLFW_INCLUDE_GLU
#include "..\OpenGL\GLFW\include\glfw3.h"


// When enabliing warning level 4, we get warning C4505: unreferenced local function has been removed
// This pragma kills this needless warning
#pragma warning(disable: 4505)


#include "MathEngine.h"
#include "VectApp.h"
#include "QuatApp.h"

#include "File.h"

#include "PCSNode.h"
#include "PCSTree.h"

//Had to change the include path in the engine properties to solve an Error with the <time.h> include in the LibOVR (Solve Later)////
#include "include\Time.h" 
#include "include\Timer.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "TextureData.h"
#include "GraphicsObject.h"
#include "GraphicsManager.h"
#include "Camera.h"
#include "NullObject.h"
#include "GraphicsObjectFileHdr.h"
#include "UserInterface.h"
#include "TextureManager.h"

//#include "GameMain.h"

#include "Object.h"
#include "GraphicModel.h"
#include "GraphicsObjectBufferHdr.h"
#include "ModelData.h"
#include "PackageHeader.h"
#include "GraphicsObject.h"
#include "ChunkHeader.h"

#include <vector>
#include <algorithm>


//#include "OVR.h"


void initializeEngine(GLFWwindow* window);
GLFWwindow* createGraphicsWindow( const char* windowName, const int Width,const int Height );
void LoadContent( void );
void Update( void );
void UnloadContent(GLFWwindow* window);

void Draw(GLFWwindow* window);

#endif 