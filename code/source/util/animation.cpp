#include "animation.h"

vec3 animationChannel::getFrame(float t){
	switch(interpolation){
		case(INT_NEAREST):
			return interpolate_nearest(keys,t);
		break;
		case(INT_LINEAR):
			return interpolate_linear(keys,t);
		break;
		case(INT_CUBIC):
			return interpolate_cubic(keys,t);
		break;
		default:
			error("unreachable code");
			return vec3(0,0,0);
		break;
	}
}

keyFrame animation::getFrame(float t){
	keyFrame KF;
	for(auto I = channels.begin(); I != channels.end(); I++){
		KF[I->first] = I->second.getFrame(t);
	}
	return KF;
}