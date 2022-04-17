#include "AIcontroller.h"
#include "inputController.h"
#include "simplemath.h"

extern eventChannel *inputChannel;
extern eventChannel *globalChannel;
AIcontroller::AIcontroller(characterController *character):character(character){
	if(!character){error("AIcontroller needs a character\n");}
	inputChannel->addListener(this);
	globalChannel->addListenerFront(this);
}
AIcontroller::~AIcontroller(){
	inputChannel->removeListener(this);
	globalChannel->removeListener(this);
}

void AIcontroller::onEvent(eventKind event){
	if(event.type == EVENT_FRAME){
		think();
	}
}

string AIcontroller::toString2(){
	string S = "";
	S += fstring("AIcontroller %p: ",this);
	if(character){
		S += fstring("char %p (",(characterController*)character);
		auto E = character->E;
		if(E){
			S+=fstring("E %p, ",(entity*)E);
			S+=fstring("name \"%s\"",E->name.c_str());
			S+=fstring("body %p",E->body);
		}else{
			S+=fstring("null");
		}
		S+=fstring(")");
	}else{
		S+= fstring("no character");
	}
	return S;
}
extern bool gamePaused;

string toString(AIState state){
    switch(state){
        case AI_STATE_NONE: return "none";
        case AI_STATE_IDLE: return "idle";
        case AI_STATE_WANDER: return "wander";
        case AI_STATE_FOLLOW: return "follow";
    }
    return "";
}

string toString(AISubstate substate){
    switch(substate){
        case AI_SUBSTATE_THINK: return "think";
        case AI_SUBSTATE_TURN: return "turn";
        case AI_SUBSTATE_WALK: return "walk";
    }
    return "";
}

void AIcontroller::think(){
    if(gamePaused){return;}
	if(!character){delete this; return;}
	auto E = character->E;
	player = inputController->character;
	if(!E){return;}
	vec3 pos = E->body->pos;
	//every frame, draw debug stuff

    //render some debug stuff
	setLayer(layerDebug);
	drawLine(pos,debugTargetPos,vec3(0,0,0));
	drawPoint(pos,vec3(0,255,0));
	drawPoint(debugTargetPos,vec3(255,0,0));
	frameprint(fstring("[AI]: state %s\tsubstate%s\n",toCString(state),toCString(substate)));
	if(state == AI_STATE_WANDER){
        int d = walkdata.path.size();
        float dist = d ? length(setZ(walkdata.path[0]->E->body->pos - pos, 0)) : 0;
        frameprint(fstring("[AI]: path: %d nodes, next in %f meters\n",d,dist));

        int j = 0;
        for(auto I = walkdata.path.begin(); I != walkdata.path.end(); I++){
            nav_node *N = *I;
            j++;
            if(!N){continue;}
            vec3 p = N->E->body->pos;
            setLayer(layerDebug);

            string text = fstring("node %d",j);
            debugFloatingText(p, text);
        }
	}
    debugFloatingText(pos+vec3(0,0,2),debugString);

	if(sleepTime){sleepTime--; return;}
    sleepTime = 10;

    debugString = fstring("[AI]: state [%s]\nsubstate [%s]\ntaskTime: %d\n",toCString(state),toCString(substate),taskTime);
	switch(state){
        case AI_STATE_NONE:
            taskTime = 0;
            state = AI_STATE_WANDER;
            substate = AI_SUBSTATE_THINK;
        break;
        case AI_STATE_IDLE:
            taskTime++;
            if(taskTime > 10){
                taskTime = 0;
                if(player){
                    if(1){
                        state = AI_STATE_FOLLOW;
                        substate = AI_SUBSTATE_THINK;
                        break;
                    }
                }
                state = AI_STATE_WANDER;
                substate = AI_SUBSTATE_THINK;
            }
        break;
        case AI_STATE_FOLLOW:
            if(!player){
                state = AI_STATE_IDLE;
                substate = AI_SUBSTATE_THINK;
                break;
            }
            if(taskTime > 10){
                taskTime = 0;
                substate = AI_SUBSTATE_THINK;
            }
            switch(substate){
                case AI_SUBSTATE_THINK:
                {
                    vec3 dest = player->E->body->pos;
                    vec3 offset = dest-pos;
                    float dist = length(offset);
                    vec3 dir = normalize(offset);
                    dest = pos+dir*max(dist-1.f,0); //destination is a bit before the player

                    if(dist < 1.f){ //if destination close, skip pathfinding.
                        walkdata.src = pos;
                        walkdata.dest = dest;
                        walkdata.path.clear();
                    }else{
                        if(length(walkdata.dest-dest) > 2.f){ //change path if the destination changed significantly since last pathfind.
                            walkdata.src = pos;
                            walkdata.dest = dest;
                            walkdata.path = AStar(pos, dest);
                        }
                        sleepTime = 0;
                    }
                    substate = AI_SUBSTATE_WALK;
                }
                break;
                case AI_SUBSTATE_WALK:
                {
                    taskTime++;
                    vec3 dest;
                    if(walkdata.path.size()){
                        nav_node *nextNode = walkdata.path[0];
                        assert(nextNode->E);
                        assert(nextNode->E->body);
                        dest = nextNode->E->body->pos;
                        debugString += fstring("node %p at %s\n",nextNode,toCString(dest));
                    }else{
                        dest = walkdata.dest;
                        debugString += fstring("final dest at %s\n",toCString(dest));
                    }
                    debugTargetPos = dest;
                    vec3 offset = setZ(dest-pos,0);
                    float dist = length(offset);
                    vec3 dir = normalize(offset);
                    vec3 angle = toVec3Angle(dir);
                    if(dist > 0.2f){    //prevent wild flipping (angular singularity at target pos)
                        character->targetDir = angle;
                    }
                    int nextSleep = 10;
                    character->targetVelocity = dir*min(walkSpeed,dist/(float)nextSleep);
                    if(dist < 0.3f){  //we arrived somewhere
                        if(walkdata.path.size()){   //we arrived at next node
                            printf("arrived at next node\n");
                            walkdata.path.erase(walkdata.path.begin()); //erase current node
                            taskTime = 0;
                            substate = AI_SUBSTATE_THINK;
                            sleepTime = 0;
                       }else{  //we arrived at destination
                            printf("arrived at destination\n");
                            character->targetVelocity = vec3(0,0,0);
                            taskTime = 0;
                            substate = AI_SUBSTATE_THINK;
                        }
                    }else{
                    }
                }
                break;
            }
        break;
        case AI_STATE_WANDER:
            debugString += fstring("%d nodes left\n",walkdata.path.size());
            if(taskTime > 10){
                taskTime = 0;
                walkdata.path.clear();
                walkdata.dest = pos;
                walkdata.src = pos;
                state = AI_STATE_IDLE;
            }
            switch(substate){
                case AI_SUBSTATE_THINK:
                {
                    int maxPathSearches = 1;
                    //1. find a random point to walk to
                    for(int I = 0; I < maxPathSearches; I++){
                        float radius = 4.f;
                        vec3 dest;
                        do{
                            float x = random(-radius,radius);
                            float y = random(-radius,radius);
                            dest = vec3(x,y,0);
                        }while(length(dest) > radius);
                        dest = pos+dest;
                        walkdata.src = pos;
                        walkdata.dest = dest;
                        nav_path path = AStar(pos,dest);
                        printf("got path: %d nodes\n",path.size());
                        printf("--------------------\n");
                        int j = 0;
                        for(auto I = path.begin(); I != path.end(); I++){
                            nav_node *n = *I;
                            printf("node %d: %p\n",j++,n);
                        }
                        printf("--------------------\n");
                        walkdata.path = path;
                        if(walkdata.path.size()){break;}
                    }
                    if(walkdata.path.size()){ substate = AI_SUBSTATE_TURN; }
                }
                break;
                case AI_SUBSTATE_TURN:
                {
                    taskTime++;
                    vec3 dest;
                    if(walkdata.path.size()){
                        nav_node *nextNode = walkdata.path[0];
                        assert(nextNode->E);
                        assert(nextNode->E->body);
                        dest = nextNode->E->body->pos;
                        debugString += fstring("node %p at %s\n",nextNode,toCString(dest));
                    }else{
                        dest = walkdata.dest;
                        debugString += fstring("final dest at %s\n",toCString(dest));
                    }
                    debugTargetPos = dest;
                    vec3 destDir = toVec3Angle(setZ(dest-pos,0));
                    character->targetDir = toVec3Angle(slerpAngle(toQuat(character->targetDir), toQuat(destDir), turnSpeed));//mix(character->targetDir, destDir, 0.3f);

                    if(length(character->targetDir - destDir) < 0.02f){
                        taskTime = 0;
                        substate = AI_SUBSTATE_WALK;
                        sleepTime = 0;
                    }else{
                    }
                }
                break;
                case AI_SUBSTATE_WALK:
                {
                    taskTime++;
                    vec3 dest;
                    if(walkdata.path.size()){
                        nav_node *nextNode = walkdata.path[0];
                        assert(nextNode->E);
                        assert(nextNode->E->body);
                        dest = nextNode->E->body->pos;
                        debugString += fstring("node %p at %s\n",nextNode,toCString(dest));
                    }else{
                        dest = walkdata.dest;
                        debugString += fstring("final dest at %s\n",toCString(dest));
                    }
                    debugTargetPos = dest;
                    vec3 offset = setZ(dest-pos,0);
                    float dist = length(offset);
                    vec3 dir = normalize(offset);
                    vec3 angle = toVec3Angle(dir);
                    if(dist > 0.2f){    //prevent wild flipping (angular singularity at target pos)
                        character->targetDir = angle;
                    }
                    int nextSleep = 10;
                    character->targetVelocity = dir*min(walkSpeed,dist/(float)nextSleep);
                    if(dist < 0.3f){  //we arrived somewhere
                        if(walkdata.path.size()){   //we arrived at next node
                            printf("arrived at next node\n");
                            walkdata.path.erase(walkdata.path.begin()); //erase current node
                            taskTime = 0;
                            substate = AI_SUBSTATE_TURN;
                            sleepTime = 0;
                            character->targetVelocity = vec3(0,0,0);
                        }else{  //we arrived at destination
                            printf("arrived at destination\n");
                            character->targetVelocity = vec3(0,0,0);
                            taskTime = 0;
                            state = AI_STATE_IDLE; //or maybe previous state for stack-based FSM?
                            substate = AI_SUBSTATE_THINK;
                        }
                    }else{
                        //sleepTime = nextSleep;
                    }
                }
                break;
            }
        break;
	}
}
