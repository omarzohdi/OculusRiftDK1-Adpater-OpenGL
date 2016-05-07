#include "AzulCore.h"

extern Camera * RenderingCamera;

extern	GLShaderManager shaderManager;
// Constructor make sure everything is initialized

Object:: Object()
{
	this->LocalToWorld.set(IDENTITY);
	this->ModelView.set(IDENTITY);
}
Object :: Object( const char * const _Name, char * const grModFile)
{
	this->setName( _Name );

	this->lightColor.set( 1.0f, 1.0f, 1.0f, 1.0f);
	this->lightPos.set(1.0f, 1.0f, 0.0f);
	this->currScale.set(1.0f, 1.0f, 1.0f);
	this->currAng.set(0.0f,0.0f,0.0f);

	this->LocalToWorld.set(IDENTITY);
	this->ModelView.set(IDENTITY);

	this->graphModel = new GraphicModel(grModFile,0);

};

Object :: Object( const char * const _Name, char * const grModFile, char * texName)
{
	this->setName( _Name );

	this->lightColor.set( 1.0f, 1.0f, 1.0f, 1.0f);
	this->lightPos.set(1.0f, 1.0f, 0.0f);
	this->currScale.set(1.0f, 1.0f, 1.0f);
	this->currAng.set(0.0f,0.0f,0.0f);

	this->LocalToWorld.set(IDENTITY);
	this->ModelView.set(IDENTITY);

	this->graphModel = new GraphicModel(grModFile, texName);

};

Object :: Object( const char * const _Name, char * const grModFile, int numTex, char ** textures)
{
	this->setName( _Name );

	this->lightColor.set( 1.0f, 1.0f, 1.0f, 1.0f);
	this->lightPos.set(1.0f, 1.0f, 0.0f);
	this->currScale.set(1.0f, 1.0f, 1.0f);
	this->currAng.set(0.0f,0.0f,0.0f);

	this->LocalToWorld.set(IDENTITY);
	this->ModelView.set(IDENTITY);

	this->graphModel = new GraphicModel(grModFile, textures, numTex);
};


void Object::setStartPos( const Vect & v)
{
	this->startPos = v;
};
	
void Object::setLightColor( const Vect & v)	
{
	this->lightColor = v;
};

void Object::setLightPos( const Vect & v)
{
	this->lightPos = v;
};

void Object::setCurrentScale(const Vect & v)
{
	this->currScale = v;
}
void Object::setCurrentAng(const Vect & v)
{
	this->currAng = v;
}


//////////////////////////////Right - Left Viewport //////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void Object::transform( void )
{
	Matrix Scale(SCALE, this->currScale[x], this->currScale[y], this->currScale[z]);
	Matrix RotX( ROT_X, this->currAng[x] );
	Matrix RotY( ROT_Y, this->currAng[y] );
	Matrix RotZ( ROT_Z, this->currAng[z] );
	Matrix Trans( TRANS, this->startPos[x], this->startPos[y],this->startPos[z]);

	this->LocalToWorld = Scale * RotY * RotX * RotZ * Trans;

//	RenderingCamera->setViewMatrix(	VRManager::getInstance().getCurrentEyeViewMatrix()	);
	//updateCamera( this->currentEye->getPosOffset());

	this->ModelView = this->LocalToWorld * RenderingCamera->getViewMatrix();
};


void Object::setRenderState( void )
{
	// Bind the texture
   glBindTexture(GL_TEXTURE_2D, this->graphModel->textureID);

	//Vect X; 
	//vrManager.getCurrentCamera()->getPos(X);

	//RenderingCamera->setProjMatrix(	VRManager::getInstance().getCurrentEyeProjMatrix()	);
   Matrix Projection = RenderingCamera->getProjMatrix();// VRManager::getInstance().getCurrentEyeProjMatrix();
	
	// Use the stock shader
	shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, 
						&ModelView,
						&Projection, 
						&this->lightPos, 
						&this->lightColor, 
						0);
};

void Object::draw( void )
{
  glBindVertexArray(this->graphModel->vao);
  glDrawElements(GL_TRIANGLES, this->graphModel->getTriangleCount()*3, GL_UNSIGNED_INT, 0);   //The starting point of the IBO

};