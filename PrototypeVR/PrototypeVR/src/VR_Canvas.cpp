#include "VR_Canvas.h"
#include <ctime>

extern Camera * RenderingCamera;

VR_Canvas::VR_Canvas()
{
	Color = Vect(255.0f,255.0f,255.0f,255.0f);
}

VR_Canvas::~VR_Canvas()
{
	glDeleteTextures(1, &this->LeftPane.TBO);
	glDeleteRenderbuffers(1, &this->LeftPane.DBO);
	glDeleteFramebuffers ( 1,  &this->LeftPane.FBO);

	glDeleteTextures(1, &this->RightPane.TBO);
	glDeleteRenderbuffers(1, &this->RightPane.DBO);
	glDeleteFramebuffers ( 1,  &this->RightPane.FBO);
}

void VR_Canvas::InitializeCanvas(VR_Avatar * inAvatar)
{
	AssignAvatar(inAvatar);
	initializeQuad();
	initalizeShaderData();
}

void VR_Canvas::AssignAvatar(VR_Avatar * inAvatar)
{
	if (inAvatar == 0)
		assert(false);

	this->assignedAvatar = inAvatar;
}

VR_Avatar * VR_Canvas::getAssignedAvatar()
{
	return this->assignedAvatar;
}

void VR_Canvas::initializeCanvasPanes()
{
	//initialize Left FrameBuffer (get data from Avatar);

	////GenerateTextureBuffer//
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &this->LeftPane.TBO);
	glBindTexture(GL_TEXTURE_2D, this->LeftPane.TBO);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->assignedAvatar->getEyeViewport(LEFT_EYE).width,
		this->assignedAvatar->getEyeViewport(LEFT_EYE).height,0,GL_RGBA,GL_UNSIGNED_BYTE,0);

	glBindTexture(GL_TEXTURE_2D, 0);

	//GenerateRenderBuffer// 
	//Depth buffer not rendering correctly on application created FBO but fine on System Created FBO//
	glGenRenderbuffers(1, &this->LeftPane.DBO);
	glBindRenderbuffer(GL_RENDERBUFFER, this->LeftPane.DBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, this->assignedAvatar->getEyeViewport(LEFT_EYE).width,
		this->assignedAvatar->getEyeViewport(LEFT_EYE).height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenFramebuffers( 1,  &this->LeftPane.FBO  );
	glBindFramebuffer( GL_FRAMEBUFFER,   this->LeftPane.FBO  );
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
		this->LeftPane.TBO, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,this->LeftPane.DBO);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	////Initialize Right FrameBuffer (get data from Avatar);
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &this->RightPane.TBO);
	glBindTexture(GL_TEXTURE_2D, this->RightPane.TBO);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->assignedAvatar->getEyeViewport(RIGHT_EYE).width,
		this->assignedAvatar->getEyeViewport(RIGHT_EYE).height,0,GL_RGBA,GL_UNSIGNED_BYTE,0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//GenerateRenderBuffer// 
	//Depth buffer not rendering correctly on application created FBO but fine on System Created FBO//
	glGenRenderbuffers(1, &this->RightPane.DBO);
	glBindRenderbuffer(GL_RENDERBUFFER, this->RightPane.DBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, this->assignedAvatar->getEyeViewport(LEFT_EYE).width,
		this->assignedAvatar->getEyeViewport(LEFT_EYE).height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenFramebuffers( 1,  &this->RightPane.FBO  );
	glBindFramebuffer( GL_FRAMEBUFFER,   this->RightPane.FBO  );
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
		this->RightPane.TBO, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,this->RightPane.DBO);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void VR_Canvas::initializeQuad()
{
	this->privTransform();			
	this->privCreateVAO();
}

void VR_Canvas::initalizeShaderData()
{
	//Move ShaderManager code here, make it less dependant from GLTools.
}

void VR_Canvas::StartLeftPaneRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->LeftPane.FBO);
}

void VR_Canvas::StartRightPaneRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->RightPane.FBO);
}

void VR_Canvas::StopRendering()
{
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void VR_Canvas::Update()
{

}
void VR_Canvas::RenderCanvas(Camera * OrthoCamera)
{
	OrthoCamera;
	privTransform();
	privRenderState(OrthoCamera);


//	this->getAssignedAvatar()->getLeftEye()->getDistort


	float DistK[4];
	float TexRange[2];
	float LensOff[2];
	float TexScale=0;
	float Aspect=0;
		

	this->ModelView = this->LocalToWorld * OrthoCamera->getViewMatrix();
		Vect lightColor( 2.5f, 2.5f, 2.5f, 1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
	
	this->getAssignedAvatar()->getLeftEye()->getDistortionRenderData(DistK, TexRange,LensOff,TexScale, Aspect);
	
	GLuint attribute_v_coord_postproc	= glGetAttribLocation(VRManager::getInstance().getShaderProgram(), "v_coord");
	GLuint uniform_fbo_texture			= glGetUniformLocation(VRManager::getInstance().getShaderProgram(), "fbo_texture");
	GLuint uniform_offset				= glGetUniformLocation(VRManager::getInstance().getShaderProgram(), "offset");
	uniform_offset;attribute_v_coord_postproc;uniform_fbo_texture;

	glUseProgram( VRManager::getInstance().getShaderProgram());
	GLfloat offset = 0;
	glUniform1f(uniform_offset, offset);

	glBindTexture(GL_TEXTURE_2D, this->LeftPane.TBO);
	glUniform1i(uniform_fbo_texture,0);
	glEnableVertexAttribArray(attribute_v_coord_postproc);

	glBindVertexArray(this->LeftPane.VAO);		
		glDrawElements(GL_TRIANGLES, 3*2, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	glDisableVertexAttribArray(attribute_v_coord_postproc);
	glBindTexture(GL_TEXTURE_2D,0);	

	glUseProgram(0);

	this->getAssignedAvatar()->getRightEye()->getDistortionRenderData(DistK, TexRange,LensOff,TexScale, Aspect);

	glUseProgram( VRManager::getInstance().getShaderProgram());	

	offset = 1;
	glUniform1f(uniform_offset, offset);

	glBindTexture(GL_TEXTURE_2D, this->RightPane.TBO);
	glUniform1i(uniform_fbo_texture,0);
	glEnableVertexAttribArray(attribute_v_coord_postproc);

	glBindVertexArray(this->RightPane.VAO);
		glDrawElements(GL_TRIANGLES, 3*2, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D,0);
	glDisableVertexAttribArray(attribute_v_coord_postproc);
	glUseProgram(0);
}
void VR_Canvas::Render(GraphicsObjectManager * gom,Camera * OrthoCamera)
{
	this->StartLeftPaneRender();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Viewport leftviewport(this->assignedAvatar->getEyeViewport(LEFT_EYE));

	glViewport(0,0,leftviewport.width,leftviewport.height);
	this->assignedAvatar->RenderLeft(gom);
	this->StopRendering();
	
	this->StartRightPaneRender();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Viewport rightviewport(this->assignedAvatar->getEyeViewport(RIGHT_EYE));

	glViewport(0,0,rightviewport.width,rightviewport.height);
	this->assignedAvatar->RenderRight(gom);
	this->StopRendering();

	//////////////////////////////////////////////////////////////////////////////////////

	this->RenderCanvas(OrthoCamera);
}
void VR_Canvas::privTransform()
{
	Matrix Scale(SCALE, 1.3f, 1.6f, 1.0f);
	
	Matrix Trans( TRANS, 0.0f, 0.0f, -5.0f);

	this->LocalToWorld = Scale * Trans;
}
void VR_Canvas::privRenderState(Camera * OrthoCamera)
{
	///////////////SetRenderState////////////////
	glViewport(0,0,1280,800);
	
	this->ModelView = this->LocalToWorld * OrthoCamera->getViewMatrix();
	Matrix mvp = this->ModelView * OrthoCamera->getProjMatrix();

	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
		
	Vect lightColor( 2.5f, 2.5f, 2.5f, 1.0f);
	Vect lightPos(0.0f, 0.0f, 5.0f);

	shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, &ModelView, &OrthoCamera->getProjMatrix(), &lightPos, &lightColor, 0);
	/////////////////////////////////////////////

}
void VR_Canvas::privCreateVAO()
{
	
	this->RightPane.tlist[0].v1 = 0;
	this->RightPane.tlist[0].v2 = 1;
	this->RightPane.tlist[0].v3 = 2;
	
	this->RightPane.tlist[1].v1 = 1;
	this->RightPane.tlist[1].v2 = 3;
	this->RightPane.tlist[1].v3 = 2;

	this->RightPane.pVerts[0].x = 2.0f;
	this->RightPane.pVerts[0].y = 1.0f;
	this->RightPane.pVerts[0].z = 1.0f;
	this->RightPane.pVerts[0].s = 1.0f;
	this->RightPane.pVerts[0].t = 1.0f;

	this->RightPane.pVerts[1].x = 0.0f;
	this->RightPane.pVerts[1].y = 1.0f;
	this->RightPane.pVerts[1].z = 1.0f;
	this->RightPane.pVerts[1].s = 0.0f;
	this->RightPane.pVerts[1].t = 1.0f;

	this->RightPane.pVerts[2].x = 2.0f;
	this->RightPane.pVerts[2].y = -1.0f;
	this->RightPane.pVerts[2].z = 1.0f;
	this->RightPane.pVerts[2].s = 1.0f;
	this->RightPane.pVerts[2].t = 0.0f;

	this->RightPane.pVerts[3].x = 0.0f;
	this->RightPane.pVerts[3].y = -1.0f;
	this->RightPane.pVerts[3].z = 1.0f;
	this->RightPane.pVerts[3].s = 0.0f;
	this->RightPane.pVerts[3].t = 0.0f;
//------------------------------------------------
	this->RightPane.pVerts[0].nx =  3.0f;
	this->RightPane.pVerts[0].ny =  5.0f;
	this->RightPane.pVerts[0].nz =  3.0f;

	this->RightPane.pVerts[1].nx = -3.0f;
	this->RightPane.pVerts[1].ny =  5.0f;
	this->RightPane.pVerts[1].nz =  3.0f;

	this->RightPane.pVerts[2].nx =  3.0f;
	this->RightPane.pVerts[2].ny = -5.0f;
	this->RightPane.pVerts[2].nz =  3.0f; 

	this->RightPane.pVerts[3].nx = -3.0f;
	this->RightPane.pVerts[3].ny = -5.0f;
	this->RightPane.pVerts[3].nz =  3.0f;
//-------------------------------------------------

	this->LeftPane.tlist[0].v1 = 0;
	this->LeftPane.tlist[0].v2 = 1;
	this->LeftPane.tlist[0].v3 = 2;
	
	this->LeftPane.tlist[1].v1 = 1;
	this->LeftPane.tlist[1].v2 = 3;
	this->LeftPane.tlist[1].v3 = 2;

	this->LeftPane.pVerts[0].x = 0.0f;
	this->LeftPane.pVerts[0].y = 1.0f;
	this->LeftPane.pVerts[0].z = 1.0f;
	this->LeftPane.pVerts[0].s = 1.0f;
	this->LeftPane.pVerts[0].t = 1.0f;

	this->LeftPane.pVerts[1].x = -2.0f;
	this->LeftPane.pVerts[1].y = 1.0f;
	this->LeftPane.pVerts[1].z = 1.0f;
	this->LeftPane.pVerts[1].s = 0.0f;
	this->LeftPane.pVerts[1].t = 1.0f;

	this->LeftPane.pVerts[2].x = 0.0f;
	this->LeftPane.pVerts[2].y = -1.0f;
	this->LeftPane.pVerts[2].z = 1.0f;
	this->LeftPane.pVerts[2].s = 1.0f;
	this->LeftPane.pVerts[2].t = 0.0f;

	this->LeftPane.pVerts[3].x = -2.0f;
	this->LeftPane.pVerts[3].y = -1.0f;
	this->LeftPane.pVerts[3].z = 1.0f;
	this->LeftPane.pVerts[3].s = 0.0f;
	this->LeftPane.pVerts[3].t = 0.0f;

//-----------------------------------------------

	this->LeftPane.pVerts[0].nx = 3.0f;
	this->LeftPane.pVerts[0].ny = 5.0f;
	this->LeftPane.pVerts[0].nz = 3.0f;
	
	this->LeftPane.pVerts[1].nx = -3.0f;
	this->LeftPane.pVerts[1].ny = 5.0f;
	this->LeftPane.pVerts[1].nz = 3.0f;

	this->LeftPane.pVerts[2].nx = 3.0f;
	this->LeftPane.pVerts[2].ny = -5.0f;
	this->LeftPane.pVerts[2].nz = 3.0f; 
	
	this->LeftPane.pVerts[3].nx = -3.0f;
	this->LeftPane.pVerts[3].ny = -5.0f;
	this->LeftPane.pVerts[3].nz = 3.0f;

//-----------------------------------------------

	glGenVertexArrays(1, &this->LeftPane.VAO);
	glBindVertexArray(this->LeftPane.VAO);
	GLuint vboL[2];
	glGenBuffers(2, vboL);

	glBindBuffer(GL_ARRAY_BUFFER, vboL[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertex_Stride) * 4, this->LeftPane.pVerts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);  
	void *offsetVert = (void *)((unsigned int)&this->LeftPane.pVerts[0].x - (unsigned int)this->LeftPane.pVerts);
	glVertexAttribPointer(0, 3, GL_FLOAT,  GL_FALSE, sizeof(QuadVertex_Stride), offsetVert);

	glEnableVertexAttribArray(3);  

	void *offsetTex = (void *)((unsigned int)&this->LeftPane.pVerts[0].s - (unsigned int)this->LeftPane.pVerts);
	glVertexAttribPointer(3, 2, GL_FLOAT,  GL_FALSE, sizeof(QuadVertex_Stride), offsetTex);
	
	glEnableVertexAttribArray(2);  
	void *offsetNorm = (void *)((unsigned int)&this->LeftPane.pVerts[0].nx - (unsigned int)this->LeftPane.pVerts);
	glVertexAttribPointer(2, 3, GL_FLOAT,  GL_FALSE, sizeof(QuadVertex_Stride), offsetNorm);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboL[1]);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(MyTriList)*2, this->LeftPane.tlist, GL_STATIC_DRAW);
	glBindVertexArray(0);

	glGenVertexArrays(1, &this->RightPane.VAO);
	glBindVertexArray(this->RightPane.VAO);
	GLuint vboR[2];
	glGenBuffers(2, vboR);

	glBindBuffer(GL_ARRAY_BUFFER, vboR[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertex_Stride) * 4, this->RightPane.pVerts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);  
	void *offsetVert2 = (void *)((unsigned int)&this->RightPane.pVerts[0].x - (unsigned int)this->RightPane.pVerts);
	glVertexAttribPointer(0, 3, GL_FLOAT,  GL_FALSE, sizeof(QuadVertex_Stride), offsetVert2);

	glEnableVertexAttribArray(3);  

	void *offsetTex2 = (void *)((unsigned int)&this->RightPane.pVerts[0].s - (unsigned int)this->RightPane.pVerts);
	glVertexAttribPointer(3, 2, GL_FLOAT,  GL_FALSE, sizeof(QuadVertex_Stride), offsetTex2);

	glEnableVertexAttribArray(2);  
	void *offsetNorm2 = (void *)((unsigned int)&this->RightPane.pVerts[0].nx - (unsigned int)this->RightPane.pVerts);
	glVertexAttribPointer(2, 3, GL_FLOAT,  GL_FALSE, sizeof(QuadVertex_Stride), offsetNorm2);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboR[1]);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(MyTriList)*2, this->RightPane.tlist, GL_STATIC_DRAW);
	glBindVertexArray(0);

}