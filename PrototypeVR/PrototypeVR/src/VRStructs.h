#ifndef VRSTRCT_H
#define VRSTRCT_H

enum EyePosition
{
	LEFT_EYE = 0,
	RIGHT_EYE
};

struct Viewport
{
	int x;
	int y;
	int width;
	int height;

	Viewport(int inX, int inY, int inWidth, int inheight)
	{
		x = inX;
		y = inY; 
		width = inWidth;
		height = inheight;
	}
	Viewport(Viewport & viewport)
	{
		x = viewport.x;
		y = viewport.y; 
		width = viewport.width;
		height = viewport.height;
	}
	void SetViewport(int inX, int inY, int inWidth, int inheight)
	{
		x = inX;
		y = inY; 
		width = inWidth;
		height = inheight;
	}
};

struct Distortion
{
	float Offset[2];
	float Scale;
	float BarrelDistortion[4];
	float ChromaticAbarration[4];

	void SetDistortion( float OFX, float OFY, float BDX, float BDY, float BDW, float BDH,
						float CAX, float CAY, float CAW, float CAH, float SCL )
	{
		Offset[0] = OFX;
		Offset[1] = OFY;

		BarrelDistortion[0] = BDX;
		BarrelDistortion[1] = BDY;
		BarrelDistortion[2] = BDW;
		BarrelDistortion[3] = BDH;

		ChromaticAbarration[0] = CAX;
		ChromaticAbarration[1] = CAY;
		ChromaticAbarration[2] = CAW;
		ChromaticAbarration[3] = CAH;

		Scale = SCL;
	}
};

#endif
