#include "convexhull.h"
#include "../util/globals.h"
#include "../math/vectors.h"
#include "../resource/models.h"
#include "../display/renderable.h"
#include "../main/control.h"
#include "../util/hook.h"
#include "../game/camera.h"
int t4phase = 0;
vector<vec> p; // point cloud;
vector<pair<idtriangle,set<int>>> shape;
set<int> *activeset;
int selectedpoint = -1;
set<int> selectedtris;
set<pair<int,int>> selectededges;
set<int> omniset;
vector<set<int>> sets;
set<int> unsorted;
int curtri = 0;

pair<vector<idtriangle>, vector<vec>> endshape;

void t4render(void *arg){ //internal for test4
	for(int I = 0; I < endshape.first.size(); I++){
		idtriangle tri = endshape.first[I];
		vec A = endshape.second[tri[0]];
		vec B = endshape.second[tri[1]];
		vec C = endshape.second[tri[2]];
		vec avg = (A+B+C)/3;
		vec N = getNormal(tri,endshape.second);
		rtriangle *rtri = new rtriangle(A,B,C,1);
		rtri->type = 2;
		rtri->colB = {0,0,0};
		rtri->colA = {0,255,0};
		scene.push_back(rtri);
		line *l = new line(avg, avg+N,1);
		l->color1 = {255,0,0};
		l->color2 = {0,255,0};
		scene.push_back(l);
	}
}

int fastforward;

int getFurthestInDir(set<int> curset, vec dir){
	if(curset.empty()){return -1;}
	double maxdot = p[*(curset.begin())].dot(dir);
	int maxI = *curset.begin();
	for(set<int>::iterator I = curset.begin(); I != curset.end(); I++){
		double dot = p[*I].dot(dir);
		if(dot > maxdot){
			maxdot = dot;
			maxI = *I;
		}
	}
	//if(maxI == -1){cout << "no furthest\n";}
	return maxI;
}

bool triangleCanSeePoint(idtriangle tri, vector<vec> p, int pt){
	vec n = getNormal(tri, p);
	return p[pt].dot(n) > p[tri[0]].dot(n);
}
// for each point in set1, if it's below the plane defined by direction N and point p1, copy to set2.//cut-and-paste to set2.

void splitByNormal(set<int> set0, set<int> &set1, set<int> &set2, idtriangle tri, vector<vec> p){//vec n, vec p1){
	//double mindot = p1.dot(n);
	for(set<int>::iterator I = set0.begin(); I != set0.end(); I++){
		if(triangleCanSeePoint(tri,p,*I))
			set1.insert(*I);
		else
			set2.insert(*I);
		/*
		if(p[*I].dot(n) < mindot){
			set2.insert(*I);
			//set1.erase(set1.begin()+I);
			//I--;
		}else
			set1.insert(*I);*/
	}
}


void joinSets(set<int> &set1, set<int> &set2){
	//set1.reserve(set1.size()+set2.size());
	set1.insert(set2.begin(), set2.end());
	//set2.clear();
}



void eraseVectorElements(vector<pair<idtriangle,set<int>>> &vec, set<int> indices, int &curtri){
	int offset = 0;
	for(set<int>::iterator I = indices.begin(); I != indices.end(); I++){
		int Idx = *I-offset;
		//cout << "erasing triangle " << *I << "/"<<Idx << "\n";
		vec.erase(vec.begin()+Idx);
		offset++;
		if(curtri >= Idx){curtri--;}
	}
}

pair<vector<idtriangle>, vector<vec>> defragmentShape(vector<pair<idtriangle,set<int>>> shape, vector<vec> p){
	

	vector<idtriangle> tris;
	vector<vec> points;
	int ptcount = 0;
	
	for(int I = 0; I < shape.size(); I++){
		
		idtriangle tri;
		for(int J = 0; J < 3; J++){
			vec sourcepoint = p[shape[I].first[J]];
			int found = -1;
			for(int T = 0; T < points.size(); T++){
				if(points[T] == sourcepoint){found = T; break;}
			}
			if(found == -1){points.push_back(sourcepoint); found = points.size()-1;}
			tri[J] = found;
		}
		tris.push_back(tri);
		
	}
	return {tris,points};
	//return pair<vector<triangle>, vector<vec>>();
}

set<int> errpoints;

void createconvexhull(){
if(t4phase == -1){
		cout << "clear\n";
		t4phase = 0;
		p.clear(); // point cloud;
		shape.clear();
		activeset = NULL; //segfault if we forgot to unhook
		selectedpoint = -1;
		selectedtris.clear();
		selectededges.clear();
		omniset.clear();
		sets.clear();
		unsorted.clear();
		t4phase = 0;
		curtri = 0;
		hook.remove("onRender","test4");
	}
	if(t4phase == 0){
		t4phase++;
		double radius = 5;
		for(int I = 0; I < 200; I++){ // create a bunch of random points
			vec V = vec(random(-radius,radius), random(-radius,radius), random(-radius,radius)); 
			if(V.length() > radius)
				{I--; continue;}
			p.push_back(camera.pos+V);
			omniset.insert(I);
		}
		activeset = new set<int>();
		*activeset = omniset;
		hook.add("onRender","test4",t4render);
		fastforward = 2;
	}if(t4phase == 1){ // make the initial two triangles
		t4phase++;
		int A = getFurthestInDir(*activeset,{0,0,1});
		activeset->erase(A);
		int B = getFurthestInDir(*activeset,{0,1,-1});
		activeset->erase(B);
		int C = getFurthestInDir(*activeset,{0,-1,-1});
		activeset->erase(C);
		shape.push_back({{A,B,C},set<int>()});
		shape.push_back({{A,C,B},set<int>()});
		splitByNormal(*activeset,shape[0].second, shape[1].second, shape[0].first, p);
	}if(t4phase == 2){ // visual cue
t4phase2:
		//cout << "2, ss = "<<shape[curtri].second.size() << "\n";
		if(curtri < shape.size()){ //if still have triangles to process, do them
			t4phase = 3;
			activeset = &(shape[curtri].second);
			//cout << "shape " << shape.size() << ", curtri " << curtri << ", shape[curtri].second " << shape[curtri].second.size() << "\n";
			//if(shape[curtri].second.size() == 1)
			//	fastforward = 0;
		}else{
			t4phase = 7; // end work
		}
	}if(t4phase == 3){ // select furthest visible point, and select all tris that can see it.
		//cout << "3, ss = "<<shape[curtri].second.size() << "\n";
		t4phase++;
		selectedpoint = getFurthestInDir(*activeset, getNormal(shape[curtri].first,p));
		//cout << "s.p. = " << selectedpoint << "\n";
		if(selectedpoint == -1){ // nothing else to add here
			selectededges.clear();
			selectedtris.clear();
			t4phase = 6;
		}else{
			for(int I = 0; I < shape.size(); I++){
				if(triangleCanSeePoint(shape[I].first,p,selectedpoint))
					selectedtris.insert(I);
				/*
				vec n = getNormal(shape[I].first,p);
				double minnormal = p[shape[I].first[0]].dot(n);
				if(p[selectedpoint].dot(n) > minnormal)
					selectedtris.insert(I);*/
			}
		}
	}if(t4phase == 4){ // delete the selected triangles, record edges.
		//cout << "4, ss = "<<shape[curtri].second.size() << ", seltris "<< selectedtris.size() << "\n";
		if(!triangleCanSeePoint(shape[curtri].first,p,selectedpoint))
		{
			cout << "ERROR! Triangle can't see point!\n";
			selectedtris.clear();
			selectedtris.insert(curtri);
			t4phase = -1;
		}else{
			t4phase++;
			int offset = 0;
			activeset = new set<int>();
			for(set<int>::iterator I = selectedtris.begin(); I != selectedtris.end(); I++){
				int A = shape[*I].first[0];
				int B = shape[*I].first[1];
				int C = shape[*I].first[2];
				if(selectededges.count({A,B}) or selectededges.count({B,A})){
					selectededges.erase({A,B});
					selectededges.erase({B,A});
				}else
					selectededges.insert({A,B});
					
				if(selectededges.count({B,C}) or selectededges.count({C,B})){
					selectededges.erase({B,C});
					selectededges.erase({C,B});
				}else
					selectededges.insert({B,C});
					
				if(selectededges.count({A,C}) or selectededges.count({C,A})){
					selectededges.erase({A,C});
					selectededges.erase({C,A});
				}else
					selectededges.insert({C,A});
				
				joinSets(*activeset, shape[*I].second);
			}
			eraseVectorElements(shape,selectedtris,curtri);
			selectedtris.clear();
			activeset->erase(selectedpoint);
		}
	}if(t4phase == 5){ // add more triangles to fill the gap
		//cout << "5, sel edges: " << selectededges.size() \
		<< ", sel point: " << selectedpoint << "\n";
t4phase5:
		idtriangle tri;
		tri[0] = selectedpoint;
		tri[1] = selectededges.begin()->first;
		tri[2] = selectededges.begin()->second;
		shape.push_back({tri,set<int>()});
		selectededges.erase(selectededges.begin());
		splitByNormal(*activeset, shape.back().second, unsorted, tri,p);
		activeset = &shape.back().second;
		t4phase++;
	}if(t4phase == 6){ // more visual cues
		if(!selectededges.empty()){
			//delete activeset;
			activeset = new set<int>();
			joinSets(*activeset, unsorted);
			unsorted.clear();
			t4phase--;
			if(fastforward > 0)
				goto t4phase5;
		}else{
			curtri++;
			t4phase = 2;
			if(fastforward > 1)
				goto t4phase2;
		}
	}if(t4phase == 7){
		activeset = new set<int>();
		*activeset = omniset;
		t4phase = -1;
		// final check
		for(vector<pair<idtriangle,set<int>>>::iterator J = shape.begin(); J != shape.end(); J++){
			splitByNormal(*activeset, errpoints, unsorted, J->first,p);
		}
		cout << "Victory! ";
		endshape.first.clear();
		endshape.second.clear();
		endshape = defragmentShape(shape, p);
		cout << endshape.first.size() << " triangles, " << endshape.second.size() << " points.\n";
	}
}