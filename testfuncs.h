void bindtests();

void testfunc1(void* arg){
	int time = 600;
	frustum frust = camera.getFrustum();
	scene.push_back(new floatingtext(frust.A, "A", time));
	scene.push_back(new floatingtext(frust.B, "B", time));
	scene.push_back(new floatingtext(frust.C, "C", time));
	scene.push_back(new floatingtext(frust.D, "D", time));
	scene.push_back(new floatingtext(frust.E, "E", time));
	scene.push_back(new floatingtext(frust.F, "F", time));
	scene.push_back(new floatingtext(frust.G, "G", time));
	scene.push_back(new floatingtext(frust.H, "H", time));
	
	printf("point A: [%f, %f, %f], H: [%f, %f, %f]\n", frust.A.x, frust.A.y, frust.A.z, frust.H.x, frust.H.y, frust.H.z);
	scene.push_back(new point(frust.A, time));
	scene.push_back(new point(frust.B, time));
	scene.push_back(new point(frust.C, time));
	scene.push_back(new point(frust.D, time));
	scene.push_back(new point(frust.E, time));
	scene.push_back(new point(frust.F, time));
	scene.push_back(new point(frust.G, time));
	scene.push_back(new point(frust.H, time));
	
	scene.push_back(new line(frust.A, frust.B, time));
	scene.push_back(new line(frust.B, frust.C, time));
	scene.push_back(new line(frust.C, frust.D, time));
	scene.push_back(new line(frust.D, frust.A, time));
	scene.push_back(new line(frust.E, frust.F, time));
	scene.push_back(new line(frust.F, frust.G, time));
	scene.push_back(new line(frust.G, frust.H, time));
	scene.push_back(new line(frust.H, frust.E, time));
	scene.push_back(new line(frust.A, frust.E, time));
	scene.push_back(new line(frust.B, frust.F, time));
	scene.push_back(new line(frust.C, frust.G, time));
	scene.push_back(new line(frust.D, frust.H, time));
	scene.push_back(new line(frust.E, frust.G, time));
	scene.push_back(new line(frust.F, frust.H, time));
	
	/*
	scene.push_back(new floatingtext(camera.pos, "Hello World!", 300));
	line *l = new line(camera.pos, camera.pos+camera.angle.forward(), 300);
	l->color1 = {255,0,0};
	l->color2 = {0,255,0};
	scene.push_back(l);
	quat testQ = quat::fromAngleAxis(1,{0,0,1});//((quat){0,{0,0,1}});
	l = new line(camera.pos+vec(0.1,0.1,0.1), camera.pos+vec(0.1,0.1,0.1)+testQ.rotateVector(camera.angle.forward()), 300);
	l->color1 = {255,0,0};
	l->color2 = {0,0,255};
	scene.push_back(l);*/
	
	/*
	frustum frust = camera.getFrustum();
	
	*/
	/*
	scene.push_back(new rtriangle(frust.A, frust.C, frust.B, 1600));
	scene.push_back(new rtriangle(frust.C, frust.A, frust.D, 1600));
	scene.push_back(new rtriangle(frust.A, frust.E, frust.D, 1600));
	scene.push_back(new rtriangle(frust.D, frust.E, frust.H, 1600));
	scene.push_back(new rtriangle(frust.E, frust.F, frust.H, 1600));
	scene.push_back(new rtriangle(frust.H, frust.F, frust.G, 1600));
	scene.push_back(new rtriangle(frust.F, frust.B, frust.G, 1600));
	scene.push_back(new rtriangle(frust.G, frust.B, frust.C, 1600));
	scene.push_back(new rtriangle(frust.A, frust.B, frust.F, 1600));
	scene.push_back(new rtriangle(frust.F, frust.E, frust.A, 1600));
	scene.push_back(new rtriangle(frust.H, frust.G, frust.D, 1600));
	scene.push_back(new rtriangle(frust.D, frust.G, frust.C, 1600));
	*/
	/*
	for(int I = 0; I<12;I++){
		cout << I<<",1: "<< frust.getTri(0,0).toString() << "\n";
		scene.push_back(new rtriangle(frust.getTri(I,0),frust.getTri(I,1),frust.getTri(I,2)));
	}
	*/
	//printf("point E distance: %f\n", (frust.H-camera.pos).length());
	
}

void testfunc2(void* arg){
	int time = 600;
	line *myline = new line(camera.pos, camera.pos+camera.angle.localX());
	myline->color2 = {255,0,0};
	myline->lifetime = time;
	scene.push_back(myline);
	scene.push_back(new floatingtext(camera.pos+camera.angle.localX(),"x",time));
	myline = new line(camera.pos, camera.pos+camera.angle.forward());
	myline->color2 = {0,255,0};
	myline->lifetime = time;
	scene.push_back(myline);
	scene.push_back(new floatingtext(camera.pos+camera.angle.localY(),"y",time));
	myline = new line(camera.pos, camera.pos+camera.angle.up());
	myline->color2 = {0,0,255};
	myline->lifetime = time;
	scene.push_back(myline);
	scene.push_back(new floatingtext(camera.pos+camera.angle.localZ(),"z",time));
	myline = new line(camera.pos, camera.pos+quat::fromAngleAxis(30,camera.angle.localZ()).rotateVector(camera.angle.localY()));
	myline->color2 = {255,255,255};
	myline->lifetime = time;
	scene.push_back(myline);
	myline = new line(camera.pos+vec(0.1,0.1,0.1), camera.pos+vec(0.1,0.1,0.1)+camera.angle.localX().cross(camera.angle.localY()));
	myline->color2 = {0,255,255};
	myline->lifetime = time;
	scene.push_back(myline);
	vec intersex;
	if(ray_triangle_intersection(camera.pos, camera.angle.forward(), vec(0,1,0),vec(0.87, -0.5,0), vec(-0.87, -0.5,0), intersex))
		scene.push_back(new point(intersex, time));
	//scene.push_back(myline);
}

void testfunc3(void *arg){
	int time = 600;
	scene.push_back(new floatingtext(camera.pos, "Hello World!", time));
	line *l = new line(camera.pos, camera.pos+camera.angle.forward(), time);
	l->color1 = {255,0,0};
	l->color2 = {0,255,0};
	scene.push_back(l);
	quat testQ = quat::fromAngleAxis(1,{0,0,1});//((quat){0,{0,0,1}});
	l = new line(camera.pos+vec(0.1,0.1,0.1), camera.pos+vec(0.1,0.1,0.1)+testQ.rotateVector(camera.angle.forward()), time);
	l->color1 = {255,0,0};
	l->color2 = {0,0,255};
	scene.push_back(l);
}


int t4phase = 0;
vector<vec> p; // point cloud;
vector<pair<triangle,set<int>>> shape;
set<int> *activeset;
set<int> omniset;
set<int> unsorted;
int selectedpoint = -1;
set<int> selectedtris;
set<pair<int,int>> selectededges;
vector<set<int>> sets;
set<int> errpoints;
int curtri = 0;
int fastforward;
pair<vector<triangle>, vector<vec>> endshape;
//static int selectedpoint = -1;
//static set<int> selectedtris;
//static set<pair<int,int>> selectededges;
//static vector<vec> pointcloud;
//static vector<vec> shapepoints;
//static vector<triangle> shape;
//static vector<vec> activeset;
//static vector<pair<triangle,vector<vec>>> sets;

void t4render(void *arg){
	for(int I = 0; I < endshape.first.size(); I++){
		triangle tri = endshape.first[I];
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
	/*
	set<int> *renderset = &omniset;
	for(set<int>::iterator I = renderset->begin(); I != renderset->end(); I++){
		int Idx = *I;
		if((Idx < 0) or (Idx > p.size())){
			cout << "Idx = " << Idx << ", p.size = " << p.size() << "\n";
			int T = 0;
			for(set<int>::iterator J = renderset->begin(); T != 25; J++){
				cout << T << ": "<< *J << "\n";
				T++;
			}
		}
		point *pt = new point(p[Idx],1);
		if(Idx == selectedpoint)
			pt->color = {0,255,255};
		if(errpoints.count(Idx))
			pt->color = {255,0,0};
		scene.push_back(pt);
	}
	for(int I = 0; I<shape.size(); I++){
		line *l = new line;
		l->end1 = p[shape[I].first[0]];
		l->end2 = l->end1 + getNormal(shape[I].first,p);
		l->color1 = {255,0,0};
		l->color2 = {0,255,0};
		l->lifetime = 1;
		scene.push_back(l);
		rtriangle *tri = new rtriangle(shape[I].first, p, 1);
		tri->type = 2;
		if(selectedtris.count(I))
			tri->colA = {255,255,0};
		else
			tri->colA = {0,255,0};
		tri->colB = {0,0,0};
		scene.push_back(tri);
	}
	for(set<pair<int,int>>::iterator I = selectededges.begin(); I != selectededges.end(); I++){
		line *l = new line(p[I->first], p[I->second],1);
		l->color1 = {0,255,255};
		l->color2 = {0,0,255};
		l->thickness = 5;
		scene.push_back(l);
	}
	*/
}

bool triangleCanSeePoint(triangle tri, vector<vec> p, int pt){
	vec n = getNormal(tri, p);
	return p[pt].dot(n) > p[tri[0]].dot(n);
}

// for each point in set1, if it's below the plane defined by direction N and point p1, copy to set2.//cut-and-paste to set2.

void splitByNormal(set<int> set0, set<int> &set1, set<int> &set2, triangle tri, vector<vec> p){//vec n, vec p1){
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

void eraseVectorElements(vector<pair<triangle,set<int>>> &vec, set<int> indices, int &curtri){
	int offset = 0;
	for(set<int>::iterator I = indices.begin(); I != indices.end(); I++){
		int Idx = *I-offset;
		//cout << "erasing triangle " << *I << "/"<<Idx << "\n";
		vec.erase(vec.begin()+Idx);
		offset++;
		if(curtri >= Idx){curtri--;}
	}
}

pair<vector<triangle>, vector<vec>> defragmentShape(vector<pair<triangle,set<int>>> shape, vector<vec> p){
	

	vector<triangle> tris;
	vector<vec> points;
	int ptcount = 0;
	
	for(int I = 0; I < shape.size(); I++){
		
		triangle tri;
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

void testfunc4(void *arg){
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
		triangle tri;
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
		for(vector<pair<triangle,set<int>>>::iterator J = shape.begin(); J != shape.end(); J++){
			splitByNormal(*activeset, errpoints, unsorted, J->first,p);
		}
		cout << "Victory! ";
		endshape.first.clear();
		endshape.second.clear();
		endshape = defragmentShape(shape, p);
		cout << endshape.first.size() << " triangles, " << endshape.second.size() << " points.\n";
	}
}
void testfunc5(void *arg){
	throw except("Hello World!");
}
void bindtests(){
	confuncs["test1"] = testfunc1;
	confuncs["test2"] = testfunc2;
	confuncs["test3"] = testfunc3;
	confuncs["test4"] = testfunc4;
	confuncs["test5"] = testfunc5;
	//confuncs["test6"] = testfunc6;
	KeyBinds["t"] = "test1";
	KeyBinds["y"] = "test2";
	KeyBinds["u"] = "test3";
	KeyBinds["i"] = "test4";
	KeyBinds["o"] = "test5";
	//KeyBinds["p"] = "test6";
	
}