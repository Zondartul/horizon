#include "gui/Gui2.h"
//Gui.h
#include <string>
#include <cstdio>
#include <math.h>
#include "util/globals.h"
#include "util/messenger.h"
#include "display/paint.h"
#include "resource/fonts.h"
#include "resource/textureloader.h"
#include "main/control.h" //bad

using std::string;

//#include "paint.h"

//color3i paintColor;
//byte paintAlpha;
vec2i mouseP; // mid-man variable for extra GUI's
/*
void setColor(color3i color)
{
	paintColor = color;
	glColor4f(paintColor.r/255.0f,paintColor.g/255.0f,paintColor.b/255.0f,paintAlpha/255.0f);
}
void setAlpha(byte alpha)
{
	paintAlpha = alpha;
	glColor4f(paintColor.r/255.0f,paintColor.g/255.0f,paintColor.b/255.0f,paintAlpha/255.0f);
}*/
GUI2base* GUI2base::lastClicked = NULL; //I hope you won't need to click two things at once.
GUI2base* GUI2base::focus = NULL; //wtfffff


















// r,g,b values are from 0 to 1
// h = [0,360], s = [0,1], v = [0,1]
//		if s == 0, then h = -1 (undefined)
vec3i RGBtoHSV(vec3i RGB)
{
	float r = RGB.x;
	float g = RGB.y;
	float b = RGB.z;
	float h = 0;
	float s = 0;
	float v = 0;
	float min, max, delta;
	min = fmin( r, fmin(g, b) );
	max = fmax( r, fmax(g, b) );
	v = max;				// v
	delta = max - min;
	if( max != 0 )
		s = delta / max;		// s
	else {
		// r = g = b = 0		// s = 0, v is undefined
		s = 0;
		h = -1;
		return {(int)h, (int)(s*255), (int)(v*255)};
	}
	if( r == max )
		h = ( g - b ) / delta;		// between yellow & magenta
	else if( g == max )
		h = 2 + ( b - r ) / delta;	// between cyan & yellow
	else
		h = 4 + ( r - g ) / delta;	// between magenta & cyan
	h *= 60;				// degrees
	if( h < 0 )
		h += 360;
	
	return {(int)h, (int)(s*255), (int)(v*255)};
}
vec3i HSVtoRGB(vec3i HSV)
{
	float r = 0;
	float g = 0;
	float b = 0;
	int i;
	float f, p, q, t;
	float h = HSV.x;
	float s = HSV.y;
	float v = HSV.z;
	h /= 60;			// sector 0 to 5
	s /= 255;
	v /= 255;
	if( s == 0 ) {
		// achromatic (grey)
		r = g = b = v;
		return {(int)(r*255),(int)(g*255),(int)(b*255)};
	}
	
	i = floor( h );
	f = h - i;			// factorial part of h
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );
	switch( i ) {
		case 0:
			r = v;
			g = t;
			b = p;
			break;
		case 1:
			r = q;
			g = v;
			b = p;
			break;
		case 2:
			r = p;
			g = v;
			b = t;
			break;
		case 3:
			r = p;
			g = q;
			b = v;
			break;
		case 4:
			r = t;
			g = p;
			b = v;
			break;
		default:		// case 5:
			r = v;
			g = p;
			b = q;
			break;
	}
	
	return {(int)(r*255),(int)(g*255),(int)(b*255)};
}










/*
void invalidate(vec2i newPos, vec2i newSize)
{
	size = newSize;
	if(parent)
	{
		newPos.x = clamp(newPos.x, parent->pos.x, parent->pos.x+parent->size.x-size.x);
		newPos.y = clamp(newPos.y, parent->pos.y, parent->pos.y+parent->size.y-size.y);
	}
	if(newPos!=pos)
	{
		pos = newPos;
		callback(arg);
	}
}
*/


/*
GUI element check list!
--GENERAL--
FIX STRATA! 
SizeToContents()
vec2i = printw
--INPUT--
Button		*
Checkbox	*
Radiobutton *
TextEntry	* (sorta)
Spinner		*
DropDownList*
ListBox		*
Slider		*
RoundSlider
ColorWheel
ColorBox	*
--NAVIGATION--
Frame		*
ScrollBar	*
Tab
--OUTPUT--
Label		*
Image		*
--WINDOWS--
Messagebox
valueDisplay*
Console
--debug--
linked list browser
ideas:
move scissor checks into base class
false scissor inherents parent rect
OR extra list of children exempt from client rect
bool clickable
callbacks for everything
dock left/right/up/down both moves and resizes element
size-to-contents and min-size.
vals = {1,2,3} should be in declaration but unimplemented by gcc.
*/


