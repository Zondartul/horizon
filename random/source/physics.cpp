#include "physics.h"
#include "btBulletDynamicsCommon.h"
//#include "btBulletCollisionCommon.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "selfaware.h"
btDefaultCollisionConfiguration* collisionConfiguration;
btCollisionDispatcher* dispatcher;
btBroadphaseInterface* overlappingPairCache;
btSequentialImpulseConstraintSolver* solver;
btDiscreteDynamicsWorld* dynamicsWorld;

#include <map>
using std::map;
vector<physbody*> physbodies;
map<physbody*, btRigidBody*> bodies;
#include "simplemath.h"
#include "glDebugDrawer.h"
#include "paint.h"
bool debugDrawEnabled = 0;
#include "terrain.h"

physbody::physbody(vec3f pos, vec3f rot, vec3f scale, model M){
	this->M = M;
	AABB aabb = M.getAABB();
	float mass = 1.0; //gotta calculate and add get/setters
	float scalar_inertia = 0.01;
	btVector3 inertia = {scalar_inertia,scalar_inertia,scalar_inertia}; //same
	btDefaultMotionState *motionState = new btDefaultMotionState();
	btBoxShape *shape = new btBoxShape(btVector3(aabb.size.x/2,aabb.size.y/2,aabb.size.z/2));
	auto info = btRigidBody::btRigidBodyConstructionInfo(mass,motionState,shape,inertia);
	btRigidBody *body = new btRigidBody(info);
	dynamicsWorld->addRigidBody(body);
	bodies[this] = body;
	physbodies.push_back(this);
	setPos(pos);
	setRot(rot);
	setScale(scale);
}


void renderPhysics(){
	if(debugDrawEnabled){
		pushRenderOptions();
		initRenderOptions();
		dynamicsWorld->debugDrawWorld();
		popRenderOptions();
	}
}

void initPhysics(){
	///-----initialization_start-----

	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = new	btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,solver,collisionConfiguration);
	GLDebugDrawer *dbgDraw = new GLDebugDrawer();
	dbgDraw->setDebugMode(
		btIDebugDraw::DBG_DrawWireframe
		//|btIDebugDraw::DBG_DrawContactPoints
		|btIDebugDraw::DBG_DrawFrames
		);
	dynamicsWorld->setDebugDrawer(dbgDraw);
	dynamicsWorld->setGravity(btVector3(0,0,-10));
	
	dynamicsWorld->getPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

}

void printPhysics(){
	printf("printPhysics: %d objects\n",dynamicsWorld->getNumCollisionObjects());
	for (int i=0;i <100; i++){
		dynamicsWorld->stepSimulation (1.f /60.f ,10) ;
		// print positions of all objects
		for(int j = dynamicsWorld->getNumCollisionObjects()-1; j >=0; j--){
			btCollisionObject * obj = dynamicsWorld -> getCollisionObjectArray ()[j];
			btRigidBody * body = btRigidBody :: upcast (obj);
			btTransform trans ;
			if ( body && body -> getMotionState ()){
				body -> getMotionState () -> getWorldTransform ( trans );

			} else{
				trans = obj -> getWorldTransform ();
			}
			printf (" world   pos  object  %d = %f ,%f ,%f\n",j, float ( trans . getOrigin (). getX ()),float (
			trans . getOrigin (). getY ()),float ( trans . getOrigin (). getZ ()));
		}
	}
}

vec3f quatToEuler(btQuaternion Q){
	float q0 = Q.x();
	float q1 = Q.y();
	float q2 = Q.z();
	float q3 = Q.w();
	
	float pitch = atan2(2*(q0*q1+q2*q3), 1-2*(q1*q1+q2*q2));
	float yaw = asin(2*(q0*q2-q3*q1));
	float roll = atan2(2*(q0*q3+q1*q2),1-2*(q2*q2+q3*q3));
	return {pitch,yaw,roll};
}

void physicstick(){
	dynamicsWorld->stepSimulation(1.f/60.f,10);
}

void physbody::setPos(vec3f pos){
	btRigidBody *b = bodies[this];
	btTransform &trans = b->getWorldTransform();
	//b->getMotionState()->getWorldTransform(trans);
	trans.setOrigin({pos.x,pos.y,pos.z});
	//b->proceedToTransform(trans);
	//b->activate();
	//b->getMotionState()->setWorldTransform(trans);
	//b->setCenterOfMassTransform(trans);
	//b->setWorldTransform(trans);
}
void physbody::setRot(vec3f rot){
	btRigidBody *b = bodies[this];
	btTransform &trans = b->getWorldTransform();
	//b->getMotionState()->getWorldTransform(trans);
	auto quat = btQuaternion();
	rot = rot*d2r;
	vec3f zero = {0,0,0};
	quat.setEuler(rot.y,rot.x,rot.z);
	trans.setRotation(quat);
	//b->proceedToTransform(trans);
	//b->activate();
	//b->getMotionState()->setWorldTransform(trans);
	//b->setCenterOfMassTransform(trans);
	//b->setWorldTransform(trans);
}
void physbody::setScale(vec3f scale){
	btRigidBody *b = bodies[this];
	btCollisionShape *s = b->getCollisionShape();
	s->setLocalScaling((btVector3){scale.x,scale.y,scale.z});
	dynamicsWorld->updateSingleAabb(b);
}
vec3f physbody::getScale(){
	btRigidBody *b = bodies[this];
	btCollisionShape *s = b->getCollisionShape();
	btVector3 scale = s->getLocalScaling();
	return (vec3f){scale.getX(),scale.getY(),scale.getZ()};
}
vec3f physbody::getPos(){
	btRigidBody *b = bodies[this];
	btTransform &trans = b->getWorldTransform();
	//motionState is used by Bullet only for graphics output and doesn't affect physics
	//b->getMotionState()->getWorldTransform(trans);
	vec3f pos = {trans.getOrigin().x(),trans.getOrigin().y(),trans.getOrigin().z()};
	return pos;
}

vec3f physbody::getRot(){
	btRigidBody *b = bodies[this];
	btTransform &trans = b->getWorldTransform();
	//b->getMotionState()->getWorldTransform(trans);
	auto m3x3 = btMatrix3x3(trans.getRotation());
	vec3f res;
	m3x3.getEulerYPR(res.x,res.y,res.z);
	vec3f rot = {res.z,res.y,-res.x};
	rot = rot/d2r;
	//vec3f rot = quatToEuler(trans.getRotation());
	return rot;
}

void physbody::setVel(vec3f vel){
	btRigidBody *b = bodies[this];
	b->setLinearVelocity({vel.x,vel.y,vel.z});
}
void physbody::setAngVel(vec3f angvel){
	btRigidBody *b = bodies[this];
	b->setAngularVelocity({angvel.x,angvel.y,angvel.z});
}
vec3f physbody::getVel(){
	btRigidBody *b = bodies[this];
	auto v = b->getLinearVelocity();
	vec3f vel = {v[0],v[1],v[2]};
	return vel;
}
vec3f physbody::getAngVel(){
	btRigidBody *b = bodies[this];
	auto v = b->getAngularVelocity();
	vec3f angvel = {v[0],v[1],v[2]};
	return angvel;
}

/*

physbody::physbody(vec3f pos, vec3f rot, model M){
	this->M = M;
	AABB aabb = M.getAABB();
	float mass = 1.0;
	btDefaultMotionState *motionState = new btDefaultMotionState();
	btBoxShape *shape = new btBoxShape(btVector3(aabb.size.x/2,aabb.size.y/2,aabb.size.z/2));
	auto info = btRigidBody::btRigidBodyConstructionInfo(mass,motionState,shape);
	btRigidBody *body = new btRigidBody(info);
	dynamicsWorld->addRigidBody(body);
	bodies[this] = body;
	physbodies.push_back(this);
	setPos(pos);
	setRot(rot);
}
*/

bool raytrace(vec3f start, vec3f end, physbody **body_out, float *distance){
	btVector3 vstart = (btVector3){start.x,start.y,start.z};
	btVector3 vend = (btVector3){end.x,end.y,end.z};
	btCollisionWorld::ClosestRayResultCallback RayCallback(vstart,vend);
	dynamicsWorld->rayTest(vstart,vend,RayCallback);
	if(RayCallback.hasHit()){
		if(body_out){
			const btCollisionObject *collisionObject = RayCallback.m_collisionObject;
			for(auto I = bodies.begin(); I != bodies.end(); I++){
				if((btCollisionObject*)(I->second) == collisionObject){
					*body_out = I->first;
					break;
				}
			}
		}
		if(distance){
			*distance = (start-end).length()*RayCallback.m_closestHitFraction;
		}
		return true;
	}else{return false;}
}

void spawnTerrain(string heightmapname, string texturename, vec3f scale,vec3f tpos){

	generateTerrain("terrain",heightmapname,texturename,scale,tpos);
	
	auto M = getModel("terrain");
	AABB aabb = M.getAABB();
	float mass = 0;
	auto motionState = new btDefaultMotionState();
	
		int heightStickWidth = 128;//2*scale.x;//128;//
		int heightStickLength = 128;//scale.y;//128;//
		
		string heightmap_path = locateResource("texture",heightmapname.c_str());
		bitmap heightmap = loadImage(heightmap_path.c_str());
		heightmap = heightmap.changeFormat(TL_LUMINANCE);
		//	auto BMP = loadImage("heightmap128"). //getTexture("heightmap128").handle.BMP;
		const void *heightfieldData = heightmap.data.get();
		if(!heightfieldData){printf("no data???");exit(0);}
		void *data2 = malloc(128*128);
		if(!data2){printf("no malloc");exit(0);}
		memcpy(data2, heightfieldData,128*128);
		printf("heightfieldData:\n%s\n",heightfieldData);
		printf("data2:\n%s\n",data2);
		btScalar heightScale = scale.z/255.0;//scale.z/512.0;//0.156;//scale.x/128.0;
		btScalar minHeight = 0;
		btScalar maxHeight = scale.z;//scale.z;
		int upAxis = 2;
		PHY_ScalarType heightDataType = PHY_UCHAR;
		bool flipQuadEdges = true;
	auto shape = new btHeightfieldTerrainShape(
		heightStickWidth, heightStickLength, data2,//heightfieldData
		heightScale, minHeight, maxHeight, 
		upAxis, heightDataType, flipQuadEdges);
		
		shape->setLocalScaling((btVector3){scale.x/128.0,scale.y/128.0,1.0});
	
	auto info = btRigidBody::btRigidBodyConstructionInfo(mass,motionState,shape);
	//info.m_friction = 1.0;
	printf("terrain friction = %f\n",info.m_friction);
	auto body = new btRigidBody(info);
	
	dynamicsWorld->addRigidBody(body);
	
	
	btTransform &posT = body->getWorldTransform();
	const btTransform &centerT = body->getCenterOfMassTransform();
	btVector3 centerTV,dummy;// = centerT.getOrigin();
	body->getAabb(centerTV,dummy);
	printf("center coords: %.3f, %.3f, %.3f\n",centerTV.getX(),centerTV.getY(), centerTV.getZ());
	posT.setOrigin((btVector3){tpos.x,tpos.y,tpos.z-centerTV.getZ()});
	//body->setWorldTransform(posT);
	
}

void toggleGravity(){
	static bool gravity = false;
	gravity = !gravity;
	if(gravity){
		printf("gravity on\n");
		dynamicsWorld->setGravity(btVector3(0,0,-10));
	}else{
		printf("gravity off\n");
		dynamicsWorld->setGravity(btVector3(0,0,0));
	}
}
