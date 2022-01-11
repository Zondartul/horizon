#include <windows.h>
#include <gl/gl.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include "../util/globals.h"
#include "../resource/textureloader.h"
using std::string;
//typedef std::string string;
FT_Library  library;
FT_Face     face;      /* handle to face object */

FT_GlyphSlot  slot;
FT_UInt       glyph_index;

int FF_version_major = FREETYPE_MAJOR;
int FF_version_minor = FREETYPE_MINOR;
int FF_version_patch = FREETYPE_PATCH;
struct glyphkind
{
    int bearingX;
    int bearingY;
    int sizeX;
    int sizeY;
    int advance;
    GLuint texture;
};

glyphkind* CurFont = NULL;

void setFont(glyphkind* F){CurFont = F;}

glyphkind* GenerateFont(const char* filepath, int size,bool aa)
{
	int error = 0;
	//if(library == NULL){ //re-initializing the freetype library hid the heisenbug again...
		error = FT_Init_FreeType( &library );
		if(error){MessageBox(0,"Couldn't init FreeType", "Error", MB_OK|MB_ICONERROR );}
	//}
	
	error = FT_New_Face( library,filepath,0,&face );
	if ( error == FT_Err_Unknown_File_Format ){MessageBox(0,"FreeType: wrong file format", "Error", MB_OK|MB_ICONERROR );}
	else if(error){MessageBox(0,"FreeType: some file error", "Error", MB_OK|MB_ICONERROR );}
	
	error = FT_Set_Char_Size(
        face,    /* handle to face object           */
        0,       /* char_width in 1/64th of points  */
        size*64,   /* char_height in 1/64th of points */
		72,     /* horizontal device resolution    */
        72 );   /* vertical device resolution      */
	if(error){MessageBox(0,"FreeType: can't set size for this font", "Error", MB_OK|MB_ICONWARNING );}
	
	glyphkind *charmap = new glyphkind[256];//(glyphkind*)malloc(256*sizeof(glyphkind));
	glEnable(GL_TEXTURE_2D);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	
	for(int I = 0; I<256; I++)
		glGenTextures(1, &charmap[I].texture);
	//MessageBox(0,"FreeType: starting texture generation...", "info", MB_OK|MB_ICONINFORMATION );
	
	for(int I = 0; I<256;I++)
	{
		error = FT_Load_Char( face, I, FT_LOAD_DEFAULT);
		if(error){
			MessageBox(0,"printing cookie...", "info", MB_OK|MB_ICONINFORMATION );
			char* str;
			sprintf(str, "FreeType: Couldn't load char %d, '%c', error = %d", I, (char)I, error);
			MessageBox(0,str, "Error", MB_OK|MB_ICONWARNING );
			PostQuitMessage(0);
			return NULL;
		}
		
		error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
		if(error){MessageBox(0,"FreeType: can't render glyph.","Error", MB_OK|MB_ICONWARNING);PostQuitMessage(0);return NULL;}
		
		int fwidth = face->glyph->bitmap.width; 
		charmap[I].sizeX = fwidth;
		int fheight = face->glyph->bitmap.rows; 
		charmap[I].sizeY = fheight;
		unsigned char *buff = face->glyph->bitmap.buffer;
		charmap[I].advance = face->glyph->metrics.horiAdvance/64;
		charmap[I].bearingX = face->glyph->metrics.horiBearingX/64;
		charmap[I].bearingY = face->glyph->metrics.horiBearingY/64;
		
		glBindTexture(GL_TEXTURE_2D, charmap[I].texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
		//glBlendColor(0.0f, 0.0f, 0.0f, 0.0f);
		unsigned char *newbuff = new unsigned char[2*fwidth*fheight];//(unsigned char*)malloc(2*fwidth*fheight*sizeof(unsigned char)); // CRASHED HERE... SOMETIMES
		if(newbuff == 0){throw except("failed to allocate character buffer @here");}		// BEFORE THIS LINE WAS ADDED
		if(aa)
		{
			for(int j = 0; j<fwidth*fheight; j++)
			{
				int a = buff[j];
				newbuff[2*j] = a;//255-buff[j];//inverting texture
				newbuff[2*j+1]=buff[j];
			}
		}else
		{
			for(int j = 0; j<fwidth*fheight; j++)
			{
				if(buff[j]>64){newbuff[2*j]=0;newbuff[2*j+1]=255;}
				else{newbuff[2*j]=255;newbuff[2*j+1]=0;}
			}
		}
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, 2, fwidth, fheight,0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, newbuff);
		free(newbuff);
	}
	return charmap;
}

/* one day...
enum printFlags
{
	ALIGN_LEFT 		= 0b0000000000;
	ALIGN_RIGHT 	= 0b0000000001;
	ALIGN_CENTER 	= 0b0000000010;
	BOLD 			= 0b0000000100;
	ITALIC			= 0b0000001000;
	STRIKEOUT		= 0b0000010000;
	UNDERLINE		= 0b0000100000;
	UPPERSCRIPT 	= 0b0001000000;
	LOWERSCRIPT		= 0b0010000000;
	CANVAS_CUT		= 0b0100000000;
	BBCODE			= 0b1000000000;
}
or just BBcode for everything...

int printw_fancy(int x, int y, pos4i canvas, string format, ...)
*/

int printw (int x, int y, int xlim, int ylim, string format, ...)
{
    if(CurFont == NULL){return -1;}

	const char *format2 = format.c_str();
    va_list args;   //  Variable argument list
    int len;        // String length
    int i;          //  Iterator
    char * text;    // Text
    va_start(args, format);
    len = _vscprintf(format2, args) + 1;
    text = (char*)malloc(len * sizeof(char));
    vsprintf(text, format2, args);
    va_end(args);

    //  Draw the characters one by one
    int x1 = x;
    int y1 = y;
    int y2 = y;
    int x2 = x;
    int x0 = x;
    glyphkind Cur;
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // the default anyway

    glEnable(GL_BLEND);
    int y0 = y+16;
	for (i = 0; text[i] != '\0'; i++)
    {
		if(text[i]=='\n'){y0 += 16;x0=x;}
		else
		{
        Cur = CurFont[text[i]];
        y1 = y0-Cur.bearingY;
        y2 = y1+Cur.sizeY;
        x1 = x0+Cur.bearingX;
        x2 = x1+Cur.sizeX;
        bool draw = true;
		if((xlim==0)||(ylim==0)){draw = false;}
		if(xlim>-1){if(x2>(x+xlim)){x0 = x; y0 += 16;}}
		y1 = y0-Cur.bearingY;
        y2 = y1+Cur.sizeY;
        x1 = x0+Cur.bearingX;
        x2 = x1+Cur.sizeX;
		if(ylim>-1){if(y2>(y+ylim)){draw = false;}}
		if(draw)
		{
		glBindTexture(GL_TEXTURE_2D, Cur.texture);
        glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex2i(x1, y1);
        glTexCoord2i(1, 0); glVertex2i(x2, y1);
        glTexCoord2i(1, 1); glVertex2i(x2, y2);
        glTexCoord2i(0, 1); glVertex2i(x1, y2);
        glEnd();
        x0 = x0+Cur.advance;
		}
		}
	}
    glDisable(GL_TEXTURE_2D);
    //glutBitmapCharacter(font_style, text[i]);

    //  Free the allocated memory for the string
    free(text);
    return x0-x; //returns the width of printed text in pixels;
}

void fontFree(glyphkind *Font)
{
    if(Font == NULL){return;}
    for(int I = 0;I<255;I++)
        glDeleteTextures(1, &Font[I].texture);

    free(Font);
}

int printwrich (int x, int y, int xlim, int ylim, int* defcolor, string format, ...)
{
    if(CurFont == NULL){return -1;}

	const char *format2 = format.c_str();
    va_list args;   //  Variable argument list
    int len;        // String length
    int i;          //  Iterator
    char * text;    // Text
    va_start(args, format);
    len = _vscprintf(format2, args) + 1;
    text = (char*)malloc(len * sizeof(char));
    vsprintf(text, format2, args);
    va_end(args);

    //  Draw the characters one by one
    int x1 = x;
    int y1 = y;
    int y2 = y;
    int x2 = x;
    int x0 = x;
    glyphkind Cur;
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // the default anyway

    glEnable(GL_BLEND);
    int y0 = y+16;
	
	//tags:
	bool tagcolor = false;
	
	for (i = 0; text[i] != '\0'; i++)
    {
		if(text[i]=='[')
		{
			if(!strncmp(text+i,"[img]",5))
			{
				i+=5;
				string name = "";
				char c = text[i];
				while((strncmp(text+i,"[/img]",6))&&(c!=0))
				{
					name+=c;
					i++;
					c = text[i];
				}
				//printf("\n::%s",name.c_str());
				if(c==0){return x0-x;}
				else
				{
					i+=5;
					int bearing = 13;//should get from font?
					texture tex = textureGet(name);
					//copypasta from PaintTexturedRect()
					glColor3f(1.0f,1.0f,1.0f);
					//glEnable( GL_TEXTURE_2D ); textures are already on
					glBindTexture( GL_TEXTURE_2D, tex.t );
					glBegin(GL_QUADS);
					glTexCoord2f(0,1);
					glVertex2i(x0, y0-bearing);
					glTexCoord2f(0,0);
					glVertex2i(x0, y0+tex.height-bearing);
					glTexCoord2f(1,0);
					glVertex2i(x0+tex.width, y0+tex.height-bearing);
					glTexCoord2f(1,1);
					glVertex2i(x0+tex.width, y0-bearing);
					glEnd();				
					//glDisable( GL_TEXTURE_2D );
					x0 = x0+tex.width;
					//y0 = y0+tex.height-13;
					continue;
				}
			}
			if(!strncmp(text+i,"[color=",7))
			{
				int hexCol = 0;
				char brk = 0;
				if(!strncmp(text+i+7,"red]",4)){glColor4f(1.0f,0.0f,0.0f,1.0f);tagcolor=true;i+=4+7;}
		else	if(!strncmp(text+i+7,"green]",6)){glColor4f(.0f,1.0f,.0f,1.0f);tagcolor=true;i+=6+7;}
		else	if(!strncmp(text+i+7,"blue]",5)){glColor4f(.0f,.0f,1.0f,1.0f);tagcolor=true;i+=5+7;}
		else	if(!strncmp(text+i+7,"yellow]",7)){glColor4f(1.0f,1.0f,.0f,1.0f);tagcolor=true;i+=7+7;}
		else	if(!strncmp(text+i+7,"orange]",7)){glColor4f(1.0f,.5f,.0f,1.0f);tagcolor=true;i+=7+7;}
		else	if(!strncmp(text+i+7,"cyan]",5)){glColor4f(.5f,.5f,1.0f,1.0f);tagcolor=true;i+=5+7;}
		else	if(!strncmp(text+i+7,"purple]",7)){glColor4f(1.0f,.0f,1.0f,1.0f);tagcolor=true;i+=7+7;}
		else	if(!strncmp(text+i+7,"pink]",5)){glColor4f(1.0f,.5f,1.0f,1.0f);tagcolor=true;i+=5+7;}
		else	if(!strncmp(text+i+7,"white]",6)){glColor4f(1.0f,1.0f,1.0f,1.0f);tagcolor=true;i+=6+7;}
		else	if(!strncmp(text+i+7,"black]",6)){glColor4f(.0f,.0f,.0f,1.0f);tagcolor=true;i+=6+7;}
		else	if(sscanf(text+i+7,"%X%c",&hexCol,&brk))
				{
				if(brk==']')
				{
				//printf("yes %x",hexCol);
				float b = hexCol%256/256.0f;
				hexCol /= 256;
				float g = hexCol%256/256.0f;
				hexCol /= 256;
				float r = hexCol%256/256.0f;
				//printf("= %f %f %f\n",r,g,b);
				glColor4f(r,g,b,1.0f);
				i = (int)(strchr(text+i,']')-text)+1;
				tagcolor = true;
				}
				}
			}
			if(!strncmp(text+i,"[/color]",8)&tagcolor)
			{
				tagcolor = false;
				glColor4f(defcolor[0]/255.0f,defcolor[1]/255.0f,defcolor[2]/255.0f,1.0f);
				i+=8;
			}
		}
		if(text[i]=='\n'){y0 += 16;x0=x;}
		else if(text[i]=='\0'){free(text); return x0-x;}
		else
		{
        Cur = CurFont[text[i]];
        y1 = y0-Cur.bearingY;
        y2 = y1+Cur.sizeY;
        x1 = x0+Cur.bearingX;
        x2 = x1+Cur.sizeX;
        bool draw = true;
		if((xlim==0)||(ylim==0)){draw = false;}
		if(xlim>-1){if(x2>(x+xlim)){x0 = x; y0 += 16;}}
		y1 = y0-Cur.bearingY;
        y2 = y1+Cur.sizeY;
        x1 = x0+Cur.bearingX;
        x2 = x1+Cur.sizeX;
		if(ylim>-1){if(y2>(y+ylim)){draw = false;}}
		if(draw)
		{
		glBindTexture(GL_TEXTURE_2D, Cur.texture);
        glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex2i(x1, y1);
        glTexCoord2i(1, 0); glVertex2i(x2, y1);
        glTexCoord2i(1, 1); glVertex2i(x2, y2);
        glTexCoord2i(0, 1); glVertex2i(x1, y2);
        glEnd();
        x0 = x0+Cur.advance;
		}
		}
	}
    glDisable(GL_TEXTURE_2D);
    //glutBitmapCharacter(font_style, text[i]);

    //  Free the allocated memory for the string
    free(text);
    return x0-x; //returns the width of printed text in pixels;
}
