#include "PhysicsHandler.h"

PhysicsHandler::PhysicsHandler() {
     broadphase = new btDbvtBroadphase();
     collisionConfiguration = new btDefaultCollisionConfiguration();
     dispatcher = new btCollisionDispatcher(collisionConfiguration);

     solver = new btSequentialImpulseConstraintSolver;

     mWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
     mWorld->setGravity(btVector3(0, -9.8, 0));
}

void PhysicsHandler::stepSimulation(float dt) {
     mWorld->stepSimulation(dt);
}

PhysicsHandler::~PhysicsHandler() {
     delete mWorld;
     delete solver;
     delete collisionConfiguration;
     delete dispatcher;
     delete broadphase;
}
