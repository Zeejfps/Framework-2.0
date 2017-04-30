#include <btBulletDynamicsCommon.h>

class PhysicsHandler {
public:
     PhysicsHandler();
     ~PhysicsHandler();
     void stepSimulation(float dt);
     
     btDiscreteDynamicsWorld* mWorld;
private:
     btBroadphaseInterface* broadphase;
     btDefaultCollisionConfiguration* collisionConfiguration;
     btCollisionDispatcher* dispatcher;
     btSequentialImpulseConstraintSolver* solver;
};
