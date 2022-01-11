#ifndef TOOLBOX_GUARD
#define TOOLBOX_GUARD
//The little window with random tools!

void genBox(void* arg);
void genCyl(void* arg);
void genCon(void* arg);
void genSph(void* arg);
void windowOpts(void* arg);
void windowPhysbody(void* arg);
void OpenMenuToolbox();
model *genCube(double l, double w, double h);
model *genPlane(double l, double w);
model *genCylinder(double height, double radius, int numsides);
model *genCone(double height, double radius, int numsides);
model *genSphere(double radius, int numverti, int numhori);
void btnGenCube(void* arg);
void btnGenCyl(void* arg);
void btnGenCone(void* arg);
void btnGenSphere(void* arg);
void genBox(void* arg);
void genCyl(void* arg);
void genCon(void* arg);
void genSph(void* arg);
void toggleWireframe(void* arg);
physBody *prevSelected;
physBody *EntSelected;
bool wPBfuncSet;
void entSelect(void *arg);
void entPush(void *arg);
void update3Dcursor(void* arg);
void toggle3Dcursor(void* arg);
void AABBupdate(void* arg);
void toggleAABB(void* arg);
void windowOpts(void* arg);
void windowPhysbodyOptions(void *arg);
void windowPhysbodyUpdate(void *arg);
void windowPhysbody(void* arg);
#endif
