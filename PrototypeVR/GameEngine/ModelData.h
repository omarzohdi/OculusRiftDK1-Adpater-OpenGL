#ifndef MODELDATA_H
#define MODELDATA_H

#include "AzulCore.h"

struct VertUVNorm
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

struct ModelTriList
{
	unsigned short v0;
	unsigned short v1;
	unsigned short v2;
};

struct TEX_Header
{
	int width;
	int height;
	int bitDepth;
};

struct CDM_Header
{
	bool incTex;
};

#endif