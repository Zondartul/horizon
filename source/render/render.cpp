#include "render.h"
#include "paint.h"

void renderable::upload(){
	setLayer(layer3D);
	uploadRmodel(rm);
	if(t){uploadTexture(t);}
}