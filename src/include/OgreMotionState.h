class OgreMotionState : public btMotionState
{
protected:
    Ogre::SceneNode* mSceneNode;
    btTransform mInitialPosition;

public:
    OgreMotionState(const btTransform &initialPosition, Ogre::SceneNode *node) {
        mSceneNode = node;
        mInitialPosition = initialPosition;
    }

    virtual ~OgreMotionState() {
    }

    void setNode(Ogre::SceneNode *node) {
        mSceneNode = node;
    }

    virtual void getWorldTransform(btTransform &worldTrans) const {
        worldTrans = mInitialPosition;
    }

    virtual void setWorldTransform(const btTransform &worldTrans) {
        if(mSceneNode == nullptr)
            return; // silently return before we set a node

        btQuaternion rot = worldTrans.getRotation();
        mSceneNode ->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());
        btVector3 pos = worldTrans.getOrigin();
        mSceneNode ->setPosition(pos.x(), pos.y(), pos.z());
    }
};
