#include "VR_Camera.h"
#include "VRManager.h"


VR_Camera::VR_Camera()
{

}
VR_Camera::~VR_Camera()
{

}

//Return Opengl Matrix DataType//
Matrix & VR_Camera::getViewMatrix(const Vect & posoffset, const Vect & lookoffset)
{
	lookoffset; posoffset;
	return this->viewMatrix;
}

Matrix & VR_Camera::getProjMatrix( const Vect & posoffset, const Vect & lookoffset )
{
	lookoffset; posoffset;
	return this->projMatrix;
}

Viewport & VR_Camera::getViewport ()
{
	return this->viewport;
}

void VR_Camera::InitializeVRCamera(const Viewport & viewport, 
					const float fov,
					const Vect &inUp, 
					const Vect &inLookAt, 
					const Vect &inPos)
{

	//Set the Viewport//
	this->viewport = viewport;
	glViewport(this->viewport.x, this->viewport.y, this->viewport.width, this->viewport.height);	

	//Set the Orientation and Position//
	this->aspectRatio = (float)this->viewport.width/(float)this->viewport.height ;
	this->fovy = fov;
	this->nearDist = 1.0f;
	this->farDist = 1024.0f;

	this->vWorldUp = inUp;

	// Point out of the screen into your EYE
	this->vDir = inPos - inLookAt;
	this->vDir.norm();

	// Clean up the vectors (Right hand rule)
	this->vRight = inUp.cross( this->vDir );
	this->vRight.norm();

	this->vUp = this->vDir.cross( this->vRight );
	this->vUp.norm();

	this->vPos = inPos;

	this->vInterest = inLookAt;

	this->privCalcPlaneHeightWidth( );
	this->privCalcFrustumVerts( );
	this->privCalcFrustumCollisionNormals( );
	this->privUpdateProjectionMatrix( Matrix());

   
	this->privUpdateViewMatrix( Vect(0.0f, 0.0f,0.0f), Matrix());
}


void VR_Camera::privCalcPlaneHeightWidth( void )
{

   this->near_height = 2.0f * tan(( this->fovy * MATH_PI/180.0f ) * .5f) * this->nearDist;
   this->near_width = this->near_height * this->aspectRatio;

   this->far_height = 2.0f * tan(( this->fovy * MATH_PI/180.0f) * .5f) * this->farDist;
   this->far_width = this->far_height * this->aspectRatio;

};

void VR_Camera::privUpdateProjectionMatrix( const Matrix &P )
{
	this->projMatrix[m0] = 2.0f * this->nearDist / this->near_width;
	this->projMatrix[m1] = 0.0f;
	this->projMatrix[m2] = 0.0f;
	this->projMatrix[m3] = 0.0f;
	
	this->projMatrix[m4] = 0.0f;
	this->projMatrix[m5] = 2.0f * this->nearDist / this->near_height;
	this->projMatrix[m6] = 0.0f;
	this->projMatrix[m7] = 0.0f;

	this->projMatrix[m8] = 0.0f;
	this->projMatrix[m9] = 0.0f;
	this->projMatrix[m10] = (this->farDist + this->nearDist)/(this->nearDist - this->farDist);
	this->projMatrix[m11] = -1.0f;

	this->projMatrix[m12] = 0.0f;
	this->projMatrix[m13] = 0.0f;
	this->projMatrix[m14] = (2.0f * this->farDist * this->nearDist)/(this->nearDist - this->farDist);
	this->projMatrix[m15] = 0.0f;

	this->projMatrix *= P;
};

void VR_Camera::privUpdateViewMatrix(  const Vect & posoffset, const Matrix &V)
{
	//This functions assumes the your vUp, vRight, vDir are still unit
	//And perpendicular to each other
	//view = Rot(orient) * trans(-(eye.basis) )

	Vect offsetpos = vPos + posoffset;

	this->viewMatrix[m0] = this->vRight[x];
	this->viewMatrix[m1] = this->vUp[x];
	this->viewMatrix[m2] = this->vDir[x];
	this->viewMatrix[m3] = 0.0f;

	this->viewMatrix[m4] = this->vRight[y];
	this->viewMatrix[m5] = this->vUp[y];
	this->viewMatrix[m6] = this->vDir[y];
	this->viewMatrix[m7] = 0.0f;

	this->viewMatrix[m8]  = this->vRight[z];
	this->viewMatrix[m9]  = this->vUp[z];
	this->viewMatrix[m10] = this->vDir[z];
	this->viewMatrix[m11] = 0.0f;


	// Change of basis (dot with the basis vectors)
	this->viewMatrix[m12]  = -offsetpos.dot(vRight);
	this->viewMatrix[m13]  = -offsetpos.dot(vUp);
	this->viewMatrix[m14] = -offsetpos.dot(vDir);
	this->viewMatrix[m15] = 1.0f;

	this->viewMatrix *= V * VRManager::getInstance().getAdaptor()->getViewMatrixAfterMovement();
	//this->viewMatrix = V * VRManager::getInstance().getAdaptor()->getViewMatrixAfterMovement();
};

void VR_Camera::privCalcFrustumVerts( void )
{
	// Top Left corner and so forth.  In this form to see the pattern
   // Might be confusing (remember the picture) vDir goes from screen into your EYE
   // so distance from the eye is "negative" vDir
	this->nearTopLeft		 = this->vPos - this->vDir * this->nearDist + this->vUp * this->near_height * 0.5f - this->vRight * this->near_width * 0.5f;
	this->nearTopRight		 = this->vPos - this->vDir * this->nearDist + this->vUp * this->near_height * 0.5f + this->vRight * this->near_width * 0.5f;
	this->nearBottomLeft	 = this->vPos - this->vDir * this->nearDist - this->vUp * this->near_height * 0.5f - this->vRight * this->near_width * 0.5f;
	this->nearBottomRight	 = this->vPos - this->vDir * this->nearDist - this->vUp * this->near_height * 0.5f + this->vRight * this->near_width * 0.5f;
	this->farTopLeft		 = this->vPos - this->vDir * this->farDist  + this->vUp * this->far_height * 0.5f  - this->vRight * this->far_width * 0.5f;
	this->farTopRight		 = this->vPos - this->vDir * this->farDist  + this->vUp * this->far_height * 0.5f  + this->vRight * this->far_width * 0.5f;
	this->farBottomLeft		 = this->vPos - this->vDir * this->farDist  - this->vUp * this->far_height * 0.5f  - this->vRight * this->far_width * 0.5f;
	this->farBottomRight   	 = this->vPos - this->vDir * this->farDist  - this->vUp * this->far_height * 0.5f  + this->vRight * this->far_width * 0.5f;
};
	
void VR_Camera::privCalcFrustumCollisionNormals( void )
{
	// Normals of the frustum around nearTopLeft
	Vect A = this->nearBottomLeft - this->nearTopLeft;
	Vect B = this->nearTopRight - this->nearTopLeft;
	Vect C = this->farTopLeft - this->nearTopLeft;
	
	this->frontNorm = A.cross(B);
	this->frontNorm.norm();

	this->leftNorm = C.cross(A);
	this->leftNorm.norm();
	
	this->topNorm = B.cross(C);
	this->topNorm.norm();

	// Normals of the frustum around farBottomRight
	A = this->farBottomLeft - this->farBottomRight;
	B = this->farTopRight - this->farBottomRight;
	C = this->nearBottomRight - this->farBottomRight;
	
	this->backNorm = A.cross(B);
	this->backNorm.norm();
	
	this->rightNorm = B.cross(C);
	this->rightNorm.norm();

	this->bottomNorm = C.cross(A);
	this->bottomNorm.norm();
};

void VR_Camera::updateCamera(const Vect & Offset, const Matrix &P, const Matrix &V)
{
   // First find the near height/width, far height/width

	this->vInterest += Offset;

	this->privCalcPlaneHeightWidth( );
	this->privCalcFrustumVerts( );
	this->privCalcFrustumCollisionNormals( );
	this->privUpdateProjectionMatrix( P );

	this->privUpdateViewMatrix( Offset, V );

}
