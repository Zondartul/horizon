#ifndef ANIMATION_GUARD
#define ANIMATION_GUARD
#include "vec.h"
#include "fancymath.h"
#include "globals.h"
#include <vector>
#include <utility>
#include <map>
#include <string>
using std::vector;
using std::pair;
using std::map;
using std::string;
//keyframe animation
//abstract system for key-framed interpolation between values.

enum animationInterpType{INT_NEAREST,INT_LINEAR,INT_CUBIC};//interpolation


typedef pair<vec3,float> animationKey;

struct animationChannel{
	//key data
	vector<animationKey> keys;
	//interpolation settings
	animationInterpType interpolation;
	vec3 getFrame(float t);
};

typedef map<string,vec3> keyFrame;

struct animation{
	map<string,animationChannel> channels;
	keyFrame getFrame(float t);
};


//an animation is a surface/heightmap/2D array, where x is bone/item name, y is time, and z/value is value/position/rotation.

//load an animation
//animation affects N out of M items (bones)
//fast-forward to time T
//get a vector for all bones

//get vector from animation1, and animation2.
//blend them (additive or alpha)
//get resulting vector

#endif