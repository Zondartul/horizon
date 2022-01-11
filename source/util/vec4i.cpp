#include "util/globals.h"

bool vec4i::contains(vec2i A)
{
	return  (A.x >= x1) && (A.x <= x2) &&
			(A.y >= y1) && (A.y <= y2);
}
bool operator == (vec4i A, vec4i B)
{
	return ((A.x1==B.x1)&&(A.y1==B.y1)&&(A.x2==B.x2)&&(A.y2==B.y2));
}
