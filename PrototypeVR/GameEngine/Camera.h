#ifndef CAMERA_H
#define CAMERA_H

#include "MathEngine.h"

class Camera
{
	enum CameraType
	{
		PERSPECTIVE_TYPE,
		ORTHOGRAPHIC_TYPE
	};


public:
	// Default constructor
   Camera();

   // Setup on single camera
	void setPerspective(const float FieldOfView_Degs, const float AspectRatio, const float NearDist, const float FarDist);
	void setViewport(const int inX, const int inY, const int width, const int height);
	void setOrientAndPosition(const Vect &Up_vect, const Vect &inLookAt_pt, const Vect &pos_pt);
	void setOrthographic(const float xMin, const float xMax, const float yMin, const float yMax, const float zMin, const float zMax);

   // update camera system
	void updateCamera(void);

   // Get the matrices for rendering
	Matrix &getViewMatrix();
	Matrix &getProjMatrix();

	
	// accessors
	void getPos( Vect &outPos) const;
	void getDir( Vect &outDir) const;
	void getUp( Vect &outUp) const;
	void getLookAt( Vect &outLookAt ) const;
	void getRight( Vect &outRight ) const;

	// Why no SETS?  Pos,Dir,Up,LookAt, Right
	//   They have to be adjust together in setOrientAndPosition()
	void setPos( const Vect &inPos);

	void setProjMatrix(const Matrix &inProjMatrix);
	void setViewMatrix(const Matrix &inViewMatrix);
			
	void getFieldOfView( float &Value) const;
	void setFieldOfView( const float Value);

	void getNearDist( float &Value) const;
	void setNearDist( const float Value);

	void getCameraUnitVector(Vect &outPos, Vect &outInterest, Vect &outTarget, Vect &outUp, Vect &outRight, Vect &outWorldUp) const;

private:  // methods should never be public
   
	void privSetViewState( void );
	void privCalcPlaneHeightWidth( void );
	void privCalcFrustumVerts( void );
	void privCalcFrustumCollisionNormals( void );
	void privUpdateProjectionMatrix( void );
	void privUpdateViewMatrix( void );

private:  // data  (Keep it private)

	// camera unit vectors (up, dir, right)
	Vect	vUp;
	Vect	vDir;
	Vect	vRight;  // derived by up and dir
	Vect	vPos;    
	Vect	vLookAt;

	// Define the frustum inputs
	float	nearDist;
	float	farDist;
	float	fovy;
	float	aspectRatio;

	// Screen size in world space
	float	near_height;
	float	near_width;
	float	far_height;
	float	far_width;

	// viewports
	int		viewport_x;
	int		viewport_y;
	int		viewport_width;
	int		viewport_height;

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

	// Projection Matrix
	Matrix	projMatrix;
	Matrix	viewMatrix;
	

	CameraType camType;
	// Ortho hacks

	float xMin;
	float yMin;
	float zMin;
	float xMax;
	float yMax;
	float zMax;

};

#endif