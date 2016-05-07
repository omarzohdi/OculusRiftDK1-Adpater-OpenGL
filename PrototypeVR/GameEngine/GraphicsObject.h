#ifndef GRAPHICS_OBJECT_H
#define GRAPHICS_OBJECT_H


class GraphicsObject : public PCSNode
{
public:
	 
	GraphicsObject()
	{
		LocalToWorld.set(IDENTITY);
		ModelView.set(IDENTITY);
	};

	virtual void transform( void ) = 0;   
	virtual void draw(void) = 0;
	virtual void setRenderState(void) = 0;

protected:
	Matrix	LocalToWorld;
	Matrix	ModelView;
	
};

#endif