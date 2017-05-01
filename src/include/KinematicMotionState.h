#include "OgreBulletUtils.h"

class KinematicMotionState : public btMotionState {
public:
    KinematicMotionState(Ogre::SceneNode* node) { this->node = node; }
    virtual ~ KinematicMotionState() { }
    virtual void getWorldTransform(btTransform &worldTrans) const {
         worldTrans = btTransform(cvt(node->getOrientation()), cvt(node->getPosition()));
    }
    virtual void setWorldTransform(const btTransform &worldTrans) {
    }

protected:
     Ogre::SceneNode* node;
};
