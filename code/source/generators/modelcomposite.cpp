#include "modelcomposite.h"
#include "modelprimitives.h"
#include "editmodel.h"

e_model *generateTree(){
	float r = 0.1f;
	float h = 0.75f;
	e_model *EM = generateCyllinder(0.75f*r,h/2.f,16);
	e_model *EM2 = generateCone(2.5*r,h,16);
	e_selection sel = EM2->selectAll();
	sel.move(vec3(0,0,h/2.f));
	sel.addTo(EM);
	e_model *EM3 = generateCone(2.0f*r,h,16);
	sel = EM3->selectAll();
	sel.move(vec3(0,0,h));
	sel.addTo(EM);
	delete EM2;
	return EM;
}