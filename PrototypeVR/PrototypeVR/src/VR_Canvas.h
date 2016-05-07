#ifndef VRCANVAS_H
#define VRCANVAS_H

#include "VR_Avatar.h"
#include "AzulCore.h"

extern GLShaderManager		shaderManager;

struct QuadVertex_Stride
{
	float x;
	float y;
	float z;
	float s;
	float t;

	float nx;
	float ny;
	float nz;
};

struct CanvasPane
{
	GLuint FBO; // FrameBuffer Object
	GLuint TBO; // TextureBuffer Object
	GLuint DBO; // DepthBuffer Object
	
	///Quad///
	GLuint	VAO; //Quad Vertex Arrray Object
	MyTriList	tlist[2]; //QuadTrianagle List;
	QuadVertex_Stride pVerts[4]; // QuadVertex List;
};

class VR_Canvas
{

public:

	VR_Canvas();
	~VR_Canvas();

	void InitializeCanvas(VR_Avatar *);
	void initializeCanvasPanes();
	void StartLeftPaneRender();
	void StartRightPaneRender();
	void StopRendering();
	void Render(GraphicsObjectManager *, Camera *);
	void Update();

	VR_Avatar * getAssignedAvatar();

private:

	///Avatar Data///
	void AssignAvatar(VR_Avatar *);
	VR_Avatar * assignedAvatar;
	
	///Shader Data///
	char * FragShader;
	char * VertShader;
	void initalizeShaderData();


	///Quad Data///
	Matrix LocalToWorld;
	Matrix ModelView;
	Vect Color;
	CanvasPane LeftPane;
	CanvasPane RightPane;
	Viewport CanvasSize;

	void initializeQuad();
	void privCreateVAO();
	void privTransform();
	void privRenderState(Camera * );
	void RenderCanvas(Camera *);

};


#endif