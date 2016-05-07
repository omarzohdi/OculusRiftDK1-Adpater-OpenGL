#include "AzulCore.h"
#include "PrototypeVR.h"
    

////////////////Non VR Dependent Cameras/////////////////
///Camera Needed to render the scene on the FrameBuffers
Camera*				RenderingCamera;
///Camera Needed to Render the Quad to the scene
Camera*				OrthoCamera;
////////////////////////////////////////////////////////

GLShaderManager		shaderManager;

void ShutdownRC(GLFWwindow* window);
void SetupRC();

void initializeEngine(GLFWwindow* window)
{
	AZUL_UNUSED( window );

	SetupRC();

	shaderManager.InitializeStockShaders();
}

void SetupRC()
{
    glClearColor(5.0f, 0.0f, 0.0f, 1.0f );

	glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    
    glCullFace(GL_BACK);
}

void ShutdownRC(GLFWwindow* window)
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void UnloadContent(GLFWwindow* window)
{
	TextureManager::deleteTextures();

	ShutdownRC(window);
}

GLFWwindow* createGraphicsWindow( const char* windowName, const int Width,const int Height )
{
	AZUL_UNUSED(Width);	
	AZUL_UNUSED(Height);
	//int vrWidth, vrHeight;

	//VRManager::getInstance().StartAdaptor();
	//VRManager::getInstance().getScreenResolution(vrWidth,vrHeight);

	if( !glfwInit() )
	{
		exit(EXIT_FAILURE);
	}
	
	//GLFWwindow* window = glfwCreateWindow(vrWidth,vrHeight, windowName , NULL, NULL);
	GLFWwindow* window = glfwCreateWindow(Width,Height, windowName , NULL, NULL);

	if (!window)
	{
		 glfwTerminate();
		 exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
    }

	return window;
}

void LoadContent( void )
{
	GraphicsObjectManager *goMgr = GraphicsObjectManager::getInstance();

	RenderingCamera = new Camera();
	
	RenderingCamera->setViewport(0,0,1280,800);
	RenderingCamera->setPerspective( 35.0f, float(1280)/float(800), 1.0f, 1024.0f);
	RenderingCamera->setOrientAndPosition( Vect(0.0f,1.0f,0.0f), Vect(-50.0f,-10.0f,-50.0f), Vect(-25.0f, 500.0f, 100.0f));	

	OrthoCamera = new Camera();
	OrthoCamera->setViewport(0,0, 1280, 800);	
	OrthoCamera ->setPerspective( 45.0f, float(1280)/float(800), 1.0f, 1000.0f);
	OrthoCamera->setOrientAndPosition( Vect(0.0f,1.0f,0.0f), Vect(0.0f,0.0f,-5.0f), Vect(0.0f,0.0f,0.0f) );


	/////////////////////VR MANAGER INITIALIZATION MAKE MORE USER FRIENDLY/////////////////////

	///Pass the Parameters -> Look at, Position and UP//
//	VRManager::getInstance().InitializeManager(Vect(0.0f,1.0f,0.0f), 
	//							 Vect(-50.0f, -10.0f, -50.0f), Vect(-25.0f,25.0f,50.0f) );

	////////////////////////////////////////////////////////////////////////////////////////////


	//RenderingCamera->setProjMatrix(	VRManager::getInstance().getCurrentEyeProjMatrix()	);
	//RenderingCamera->setProjMatrix(	VRManager::getInstance().getCurrentEyeViewMatrix()	);


	Object *Pyramid = new Object( "Pyramid","Models/Pyramid.cdm");
	Pyramid->setStartPos( Vect(-50.0f, -10.0f, -250.0f) );
	Pyramid->setCurrentScale( Vect(1.0f, 1.0f, 1.0f) );
	Pyramid->setCurrentAng( Vect (-1.5f, 0.0f, 0.0f) );
	goMgr->addObject( Pyramid );

	Object *House = new Object( "House","Models/house.cdm");
	House->setStartPos( Vect(100.0f, -10.0f, -250.0f) );
	House->setCurrentScale( Vect(0.05f, 0.05f, 0.05f) );
	House->setCurrentAng( Vect (0.0f, 0.0f, 0.0f) );
	goMgr->addObject( House );

	Object *Scifi = new Object( "SciFi","Models/scifi.cdm");
	Scifi->setStartPos( Vect(-250.0f, -10.0f, -50.0f) );
	Scifi->setCurrentScale( Vect(1.0f, 1.0f, 1.0f) );
	Scifi->setCurrentAng( Vect (0.0f, 0.0f, 0.0f) );
	goMgr->addObject( Scifi );

	Object *CityBlock = new Object( "CityBlock","Models/CityBlock.cdm");
	CityBlock->setStartPos( Vect(-50.0f, -10.0f, -50.0f) );
	CityBlock->setCurrentScale( Vect(0.01f, 0.01f, 0.01f) );
	CityBlock->setCurrentAng( Vect (0.0f, 0.0f, 0.0f) );
	goMgr->addObject( CityBlock );

	Object *LargeCity = new Object( "LargeCity","Models/LargeCity.cdm");
	LargeCity->setStartPos( Vect(250.0f, -10.0f, -50.0f) );
	LargeCity->setCurrentScale( Vect(0.01f, 0.01f, 0.01f) );
	LargeCity->setCurrentAng( Vect (0.0f, 0.0f, 0.0f) );
	goMgr->addObject( LargeCity );
}

void Update( void )
{
	static Time tCurrent = 10*Time(TIME_NTSC_30_FRAME);

	//VRManager::getInstance().update();
	RenderingCamera->updateCamera();
	OrthoCamera->updateCamera();
};

void Draw(GLFWwindow* window)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 
	glClearColor(0.0f, 0.5f, 0.5f, 1.0f );



	GraphicsObjectManager *gom = GraphicsObjectManager::getInstance();
	gom->drawObjects();

//	VRManager::getInstance().render(gom,OrthoCamera);

	glfwSwapBuffers(window);
	glfwPollEvents();

}