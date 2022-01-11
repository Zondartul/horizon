#include "physCharacter.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "BulletCollision/CollisionShapes/btCapsuleShape.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include <map>
using std::map;
map<physCharacter*,btKinematicCharacterController*> controllers;
#include "btBulletDynamicsCommon.h"
extern btDiscreteDynamicsWorld* dynamicsWorld;
#include "simplemath.h"
#include "globals.h"

physCharacter::physCharacter(){
	//init struct
	pos = (vec3f){0,0,0};
	fly = false;
	//initialize controller
	btCapsuleShape *shape = new btCapsuleShapeZ(0.1,0.5); //all capsules are axis-aligned in Bullet
	if(!shape){error("no shape");}	
	btPairCachingGhostObject *ghostObject = new btPairCachingGhostObject();
	if(!ghostObject){error("no ghostObject");}
	ghostObject->setCollisionShape(shape);
	ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	
	btScalar stepHeight = 0.1;
	int upAxis = 2;
	btKinematicCharacterController *charController = 
	new btKinematicCharacterController(
		ghostObject, 
		shape,
		stepHeight,
		upAxis
	);
	//if(!charController){printf("cant make a character controller!\n");exit(0);}
	if(!charController){error("no charController");}
	printf("dynamics world = %p\n",dynamicsWorld);	
	dynamicsWorld->addCollisionObject(ghostObject,btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
	dynamicsWorld->addAction(charController);
	
	// auto trans = ghostObject->getWorldTransform();
	// trans.setRotation(btQuaternion(1,1,1,90*d2r));
	// ghostObject->setWorldTransform(trans);
	
	// btTransform &trans = ghostObject->getWorldTransform();
	// auto quat = btQuaternion();
	// vec3f rot = {10,20,30};
	// rot = rot*d2r;
	// vec3f zero = {0,0,0};
	// quat.setEuler(rot.y,rot.x,rot.z);
	// trans.setRotation(quat);
	
	controllers[this] = charController;
	printf("new character %p: controller %p\n",this,controllers[this]);
	
}
void physCharacter::setWalkDir(vec3f vel){
	auto controller = controllers[this];
	if(fly){
		controller->setWalkDirection((btVector3){vel.x,vel.y,vel.z});
	}else{
		controller->setWalkDirection((btVector3){vel.x,vel.y,0});
	}
	pos = pos+vel;
}
void physCharacter::toggleFly(){
	auto controller = controllers[this];
	if(fly){
		fly = false;
		controller->setFallSpeed(55.0f);
	}else{
		fly = true;
		controller->setFallSpeed(0.0f);
	}
}
void physCharacter::setVel(vec3f vel){ //bork bork?
	auto controller = controllers[this];
	controller->setFallSpeed(0);
	setWalkDir(vel);
	controller->setFallSpeed(55.0f);
}
void physCharacter::jump(float height){
	auto controller = controllers[this];
	if(fly){
		controller->setJumpSpeed(0.1);
		controller->jump();
	}else if(controller->onGround()){
		controller->setJumpSpeed(5);//(0.1);
		//controller->setMaxJumpHeight(0.0);//(1.0);
		controller->jump();
	}
}
void physCharacter::setPos(vec3f pos){
	auto controller = controllers[this];
	printf("(c[%p]==%p)->setPos(%f,%f,%f)\n",this, controllers[this],pos.x,pos.y,pos.z);
	controller->warp((btVector3){pos.x,pos.y,pos.z});
	this->pos = pos;
}
void physCharacter::setRot(vec3f rot){
}
void physCharacter::setSize(vec3f size){
}
bool physCharacter::onGround(){
	auto controller = controllers[this];
	return controller->onGround();
}
vec3f physCharacter::getEyePos(){
	return getPos()+(vec3f){0,0,0.05f};
}
vec3f physCharacter::getPos(){
	auto controller = controllers[this];
	auto V = controller->getGhostObject()->getWorldTransform().getOrigin();
	return (vec3f){V.getX(),V.getY(),V.getZ()};
}
