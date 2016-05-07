#ifndef VRCAMERA_H
#define VRCAMERA_H

#include "AzulCore.h"
#include "VRStructs.h"

class VR_Camera
{

public:
	VR_Camera();
	~VR_Camera();

	void updateCamera(const Vect & Offset, const Matrix &P, const Matrix &V);

	///change to use OpenGL Vector Class//
	void InitializeVRCamera(const Viewport & viewport, 
					   const float aspectratio,
					   const Vect &inUp, 
					   const Vect &inLookAt, 
					   const Vect &inPos);					  ;

	Matrix & getProjMatrix( const Vect & posoffset, const Vect & lookoffset );
	Matrix & getViewMatrix( const Vect & posoffset, const Vect & lookoffset );
	Viewport & getViewport();

private:

	void privCalcFrustumVerts( void );
	void privCalcFrustumCollisionNormals( void );
	void privUpdateProjectionMatrix(const Matrix &P);
	void privUpdateViewMatrix( const Vect & posoffset, const Matrix &V );
	void privCalcPlaneHeightWidth(void );

	Viewport viewport;
	Vect  vInterest;
	Vect	vUp;
	Vect	vPos;
	Vect	vDir;
	Vect	vRight;  // derived by up and dir
	Vect  vWorldUp; // up vector in the world coordinate

	float	nearDist;
	float	farDist;
	float	fovy;
	float	aspectRatio;

	float	near_height;
	float	near_width;
	float	far_height;
	float	far_width;

	Matrix	projMatrix;
	Matrix	viewMatrix;


	// world space coords for viewing frustum
	Vect	nearTopLeft;
	Vect	nearTopRight;
	Vect	nearBottomLeft;
	Vect	nearBottomRight;	
	Vect	farTopLeft;
	Vect	farTopRight;
	Vect	farBottomLeft;
	Vect	farBottomRight;

	// Normals of the frustum
	Vect	frontNorm;
	Vect	backNorm;
	Vect	rightNorm;
	Vect	leftNorm;
	Vect	topNorm;
	Vect	bottomNorm;

};

#endif