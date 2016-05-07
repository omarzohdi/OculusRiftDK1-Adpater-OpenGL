#include "VRManager.h"
#include "VR_Calibrator.h"
#include "VR_Canvas.h"
//
//////////////////////Public//////////////////
//

VRManager &VRManager::getInstance()
{
	static VRManager instance;

	return instance;
}

VRManager::VRManager(void)
{

}

VRManager::~VRManager(void)
{
	delete Adaptor;
}

LibOVR_Adaptor * VRManager::getAdaptor(void)
{
	return this->Adaptor;
}
void VRManager::InitializeCamera(VR_Avatar & Avatar,
								 const Viewport & Vp, const float fov, const Vect & Up,const Vect & Pos,const Vect & Lookat)
{
		Avatar.InitializeCamera(Vp, fov, Up, Pos, Lookat);
}
void VRManager::StartAdaptor()
{
	this->Adaptor = new LibOVR_Adaptor();
}
void VRManager::InitializeManager(const Vect & Up,const Vect & Pos,const Vect & Lookat)
{	
	this->InitializeCanvas();
	this->InitializeAvatars();

	float FovY, AspRatio;
	this->Adaptor->getViewportPrespectiveData(FovY, AspRatio);

	this->InitializeCamera( *(this->getCurrentCanvas().getAssignedAvatar()), 
							Viewport(0,0,this->Adaptor->getHorizonatalResolution()/2,this->Adaptor->getVerticalResolution()), 
							FovY,Up, Pos,Lookat);
	this->InitializeEyes();
	this->InitializeCanvasPanes();
	this->InitializeShader();
	this->InitializeCalibrationTool();
}
void VRManager::InitializeCanvasPanes()
{
	this->CurrentCanvas->initializeCanvasPanes();
}
void VRManager::InitializeEyes()
{
	for (int i=0; i < AVATARS; ++i)
	{
		this->CurrentCanvas->getAssignedAvatar()->InitializeEyes();
	}
}
void VRManager::InitializeCanvas()
{
	this->CanvasList = new VR_Canvas[AVATARS];
	this->CurrentCanvas = &this->CanvasList[0];
}

void VRManager::InitializeAvatars()
{
	this->AvatarsList = new VR_Avatar[AVATARS];

	this->CurrentCanvas->InitializeCanvas(&AvatarsList[0]);
}

void VRManager::InitializeCalibrationTool()
{
	this->Calibrator = new VR_Calibrator();
}

void VRManager::InitializeShader()
{
	 GLint Error;
	const char *postprocessFS[1];
	const char *postprocessVS[1];
	
	GLuint VertexShader;
	GLuint FragShader;



									
	postprocessVS[0] =	
		/*				"attribute vec4 vVertex;"
						"void main(void) "
						"{ "
							"gl_Position = vVertex; "
						"}";*/
		"#version 150 \n"
						"attribute vec2 v_coord;"
						"uniform sampler2D fbo_texture;"
						"varying vec2 f_texcoord;"
						"void main(void) {"
						"gl_Position = vec4(v_coord, 0.0, 1.0);"
						"f_texcoord = ((v_coord + 1.0) / 2.0 );"
						"f_texcoord.x *= 2.0;"
						"}"; // This works but the shader applies same texture twice, not good
							// we want the coordinates from the texture to be translated to half 
							
	
	postprocessFS[0] =	/*"uniform vec4 vColor;"
						"void main(void) "
						"{ gl_FragColor = vColor;"
						"}";
						
	/*"#version 150 \n"
						"uniform sampler2D fbo_texture;"
						"uniform float offset;"
						"varying vec2 f_texcoord;"
 
						"void main(void) {"
						  "vec2 texcoord = f_texcoord;"
						  "texcoord.x += sin(texcoord.y * 4*2*3.14159 + offset) / 100;"
						  "gl_FragColor = texture2D(fbo_texture, texcoord);"
						"}";*/
	"#version 150 \n"
						"uniform sampler2D fbo_texture;"
						"varying vec2 f_texcoord;"
						"void main(void) {"
						"gl_FragColor = texture2D(fbo_texture, f_texcoord);"
						"}";

						

	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1 , (const GLchar **)  postprocessVS , 0 );
	glCompileShader(VertexShader);
	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Error);
	assert(Error);

	FragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragShader , 1 , (const GLchar **)  postprocessFS , 0 );
	glCompileShader(FragShader);
	glGetShaderiv(FragShader, GL_COMPILE_STATUS, &Error);
	assert(Error);

	ShaderProgram = glCreateProgram();

	glAttachShader (ShaderProgram, VertexShader);
	glAttachShader (ShaderProgram, FragShader);

	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Error);
	assert(Error);
	
	/*
	if(!Error)
	{
		char * ErrorLog = new char[500];
		glGetShaderInfoLog(ShaderProgram,500, NULL, ErrorLog);	
		
	}*/
	
	
	
	glDeleteShader(VertexShader);
	glDeleteShader(FragShader);
	

}

GLuint VRManager::getShaderProgram()
{
	return this->ShaderProgram;
}

void VRManager::getScreenResolution(int &HorRez, int &VerRez)
{
	HorRez = this->Adaptor->getHorizonatalResolution();
	VerRez = this->Adaptor->getVerticalResolution();
}

void VRManager::getViewportSize(int &width, int &height)
{
	width = this->Adaptor->getHorizonatalResolution()/2;
	height = this->Adaptor->getVerticalResolution();
}

void VRManager::update()
{
	if (this->Calibrator->isCalibratorOn())
		this->Calibrate(this->CurrentCanvas->getAssignedAvatar());

	////Only updates one avatar ( only one oculus can be connected at one time)
	//// this will be solved in a future iteration
	this->CurrentCanvas->getAssignedAvatar()->Update();
	this->sensorUpdate();
}
void VRManager::sensorUpdate()
{	
	double curtime = this->Adaptor->getVRTime();
	this->Adaptor->setVRLastUpdate( curtime );

    if (this->Adaptor->isSensorInit())
    {        
		this->Adaptor->CalculateYawPitchRoll();
    }

//	this->UpdateSensorRenderData();
}

void VRManager::UpdateSensorRenderData()
{
	Vect Up,Dir;

	Up = this->CurrentCanvas->getAssignedAvatar()->currentEye->getUp();
	Dir = this->CurrentCanvas->getAssignedAvatar()->currentEye->getDir();

	this->CurrentCanvas->getAssignedAvatar()->currentEye->setViewMatrix(
		this->Adaptor->getViewMatrixAfterMovement());
}

void VRManager::render(GraphicsObjectManager * gom,Camera * OrthoCamera)
{
	this->CurrentCanvas->Render(gom,OrthoCamera);
}

VR_Canvas & VRManager::getCurrentCanvas()
{
	return  *CurrentCanvas;
}

Matrix & VRManager::getCurrentEyeViewMatrix(  )
{
	return this->CurrentCanvas->getAssignedAvatar()->currentEye->getoffsetViewMatrix();
}
Matrix & VRManager::getCurrentEyeProjMatrix( )
{
	return this->CurrentCanvas->getAssignedAvatar()->currentEye->getoffsetPorjectionMatrix();
}

bool VRManager::isCalibrationOn()
{
	return this->Calibrator->isCalibratorOn();
}

void VRManager::ActivateCalibrationMode()
{
	this->Calibrator->setCalibrating(true);
}

void VRManager::DeactivateCalibrationMode()
{
	this->Calibrator->setCalibrating(false);
}

void VRManager::Calibrate(VR_Avatar * inAvatar)
{
	this->Calibrator->Calibrate(inAvatar);
}

void VRManager::setCalibrationAmount(float inAmount)
{
	this->Calibrator->setCalibrationAmount(inAmount);
}

void VRManager::SwitchCalibrationMode(int key)
{
	this->Calibrator->SwitchCalibrationMode(key);
}