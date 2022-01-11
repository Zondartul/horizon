#ifndef CONVEXHULL_GUARD
#define CONVEXHULL_GUARD
#include "../util/globals.h"
#include "../math/vectors.h"
#include "../resource/models.h"
extern int t4phase;
extern vector<vec> p;
extern vector<pair<idtriangle,set<int>>> shape;
extern set<int> *activeset;
extern int selectedpoint;
extern set<int> selectedtris;
extern set<pair<int,int>> selectededges;
extern set<int> omniset;
extern vector<set<int>> sets;
extern set<int> unsorted;
extern int curtri;
extern pair<vector<idtriangle>, vector<vec>> endshape;
void t4render(void *arg);
extern int fastforward;
int getFurthestInDir(set<int> curset, vec dir);
bool triangleCanSeePoint(idtriangle tri, vector<vec> *p, int pt);
void splitByNormal(set<int> set0, set<int> &set1, set<int> &set2, idtriangle tri, vector<vec> *p);
void joinSets(set<int> &set1, set<int> &set2);
void eraseVectorElements(vector<pair<idtriangle,set<int>>> &vec, set<int> indices, int &curtri);
pair<vector<idtriangle>, vector<vec>> defragmentShape(vector<pair<idtriangle,set<int>>> shape, vector<vec> *p);
extern set<int> errpoints;
void createconvexhull();
#endif