// Texture ///////////////////////////////////////////////////////////
#include "textureloader.h"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glext.h>
#include <stdio.h>    //  Standard Input\Output C Library
//#include <stdarg.h>   //  To use functions with variables arguments
//#include <stdlib.h>   //  for malloc
//#include <gl/glut.h>  //  Include GLUT, OpenGL, and GLU libraries
//#include <string.h>
#include "../util/globals.h"

texture::texture(){
	t = 0;
	name = "no_texture";
	width = 0;
	height = 0;
}
void texture::free(){
	glDeleteTextures(1,&t);
	t = 0;
	width = 0;
	height = 0;
	name = "no_texture";
}
	texture::~texture(){
	//free();
	//No. we're making lots of temporary copies.
}
texture notexture(){texture notexture;return notexture;}

vector<texture> AllTextures;


texture textureGet(string name)
{
	for(int i = 0; i<AllTextures.size();i++)
	{
		if(AllTextures[i].name == name)
		{
			return AllTextures[i];
		}
	}
	return notexture();
}

// load a 256x256 RGB .RAW file as a texture
GLuint LoadTextureRAW( const char * filename, int wrap )
{
  GLuint tex;
  int width, height;
  BYTE * data;
  FILE * file;

  // open texture data
  file = fopen( filename, "rb" );
  if ( file == NULL ) {printf("texture file not found: %s",filename);return 0;}

  // allocate buffer
  width = 256;
  height = 256;
  data = (BYTE*)malloc( width * height * 3 );

  // read texture data
  fread( data, width * height * 3, 1, file );
  fclose( file );

  // allocate a texture name
  glGenTextures( 1, &tex );

  // select our current texture
  glBindTexture( GL_TEXTURE_2D, tex );

  // select modulate to mix texture with color for shading
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

  // when texture area is small, bilinear filter the closest MIP map
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                   GL_LINEAR_MIPMAP_NEAREST );
  // when texture area is large, bilinear filter the first MIP map
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

  // if wrap is true, the texture wraps over at the edges (repeat)
  //       ... false, the texture ends at the edges (clamp)
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                   wrap ? GL_REPEAT : GL_CLAMP );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                   wrap ? GL_REPEAT : GL_CLAMP );

  // build our texture MIP maps
  gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width,
    height, GL_RGB, GL_UNSIGNED_BYTE, data );

  // free buffer
  free( data );
  printf("texture loaded: %s",filename);
  return tex;
}
bool texture_load_pixelated = 0;

texture GenTextureBMP( string filename)
{
  GLuint tex;
  int width, height;
  char depth;
  BYTE * data;
  BYTE * newdata;
  FILE * file;

  // open texture data
  file = fopen( filename.c_str(), "rb" );
  if ( file == NULL ) {printf("texture file not found: %s",filename.c_str());return notexture();}

  // allocate buffer
  fseek(file,0x12,SEEK_SET);//0x12H
  fread(&width,4,1,file);
  fread(&height,4,1,file);
  fseek(file,0x1C,SEEK_SET);//0x1CH
  fread(&depth,1,1,file);
  printf("width x height x depth = %d x %d x %d\n", width, height, depth);
  //width = 256;
  //height = 256;
  data = (BYTE*)malloc( width * height * depth/8 );
  
  
  
  // read texture data
  fseek(file,0x36,SEEK_SET);
  fread( data, width * height * depth/8, 1, file );
  fclose( file );
  
  //image manipulation magics happen here
  newdata = (BYTE*)malloc( width * height * (depth/8+1) );
  for(int i = 0;i<(width*height);i++)
  {
	BYTE r,g,b;
	newdata[i*4] = b = data[i*3];
	newdata[i*4+1] = g = data[i*3+1];
	newdata[i*4+2] = r = data[i*3+2];
	if((r==255)&&(b==255)&&(g==0))//pink = transparent
	{newdata[i*4+3] = 0;}
	else
	{newdata[i*4+3] = 255;}
  }
  // allocate a texture name
  glGenTextures( 1, &tex );

  // select our current texture
  glBindTexture( GL_TEXTURE_2D, tex );

  // select modulate to mix texture with color for shading
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

  // when texture area is small, bilinear filter the closest MIP map
  if(texture_load_pixelated)
  {
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_NEAREST);
	
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  }
  else
  {
   glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_NEAREST );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  }
  // when texture area is large, bilinear filter the first MIP map
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

  // if wrap is true, the texture wraps over at the edges (repeat)
  //       ... false, the texture ends at the edges (clamp)
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);

  // build our texture MIP maps
  //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  
  gluBuild2DMipmaps( GL_TEXTURE_2D, 4, width, //HUH
    height, GL_BGRA, GL_UNSIGNED_BYTE, newdata );

  // free buffer
  free( data );

  // make a texture holder
  texture newTex;
  newTex.t = tex;
  newTex.name = filename;
  newTex.width = width;
  newTex.height = height;
  printf("texture loaded: %s",filename.c_str());
  return newTex;
}

GLuint LoadTextureBMP( const char * filename)
{
  GLuint tex;
  int width, height;
  char depth;
  BYTE * data;
  FILE * file;

  // open texture data
  file = fopen( filename, "rb" );
  if ( file == NULL ) {printf("texture file not found: %s",filename);return 0;}

  // allocate buffer
  fseek(file,0x12,SEEK_SET);//0x12H
  fread(&width,4,1,file);
  fread(&height,4,1,file);
  fseek(file,0x1C,SEEK_SET);//0x1CH
  fread(&depth,1,1,file);
  printf("width x height x depth = %d x %d x %d\n", width, height, depth);
  //width = 256;
  //height = 256;
  data = (BYTE*)malloc( width * height * depth/8 );

  // read texture data
  fseek(file,0x36,SEEK_SET);
  fread( data, width * height * depth/8, 1, file );
  fclose( file );

  // allocate a texture name
  glGenTextures( 1, &tex );

  // select our current texture
  glBindTexture( GL_TEXTURE_2D, tex );

  // select modulate to mix texture with color for shading
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

  // when texture area is small, bilinear filter the closest MIP map
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                   GL_LINEAR_MIPMAP_NEAREST );
  // when texture area is large, bilinear filter the first MIP map
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

  // if wrap is true, the texture wraps over at the edges (repeat)
  //       ... false, the texture ends at the edges (clamp)
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);

  // build our texture MIP maps
  gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, //HUH
    height, GL_RGB, GL_UNSIGNED_BYTE, data );

  // free buffer
  free( data );
  printf("texture loaded: %s",filename);
  return tex;

}

void FreeTexture( GLuint tex )
{

  glDeleteTextures( 1, &tex );

}
