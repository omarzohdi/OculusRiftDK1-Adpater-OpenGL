#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "AzulCore.h"

struct TextureIDs
{
   TextureIDs  *next;
   GLuint      texID;
};


class TextureManager
{
public:
 //  static TextureManager *getInstance( void ); 
   static GLuint addTexture( TextureData &textData, int numTex );
   static GLuint addTexture( BufferData &textData );
   static void deleteTextures( void );
   static GLuint loadBuffer(BufferData &textData, char * buffer);
   static GLuint loadFile(TextureData &textData, int numTex);

private:
   static TextureManager *privGetInstance( void );
   GLuint privAddTexture( TextureData &textData, int numTex );
   GLuint privAddTexture( BufferData &textData );
   void   privDeleteTextures( void );

   TextureManager(void);

   // single link list of all the textureIDs
   TextureIDs *head;
   int         textureCount; 

};


#endif