#ifndef GRAPHICMODEL_H
#define GRAPHICMODEL_H

#include "AzulCore.h"
#include <vector>
#include "ModelData.h"

struct MyVertex_stride
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

struct MyTriList
{
	unsigned short v1;
	unsigned short v2;
	unsigned short v3;
};

class GraphicModel
{

private:
		std::vector<VertUVNorm> model_vert;
		std::vector<ModelTriList> model_trilist;

public:
		GLuint vao;
		GLuint textureID;

private:
		void loadDatafromFile(char * filename, char * texName, int numTex);
		void loadTexture(char * texName, int numTex);
		
		void loadTexturefromBuffer( char * buffer, TEX_Header texHeader);
		void createVAO( void );
		MyVertex_stride * ConvertVertextoVao( void );
		MyTriList		* ConvertVBOTritoTrilist( void );

public:
		GraphicModel(char * filename, char * texName);
		GraphicModel(char * filename, char ** texName, int numTex );
		int getTriangleCount ();
		int getVBOCount();
};


#endif