#ifndef MY_GAME_H
#define MY_GAME_H

#include "OgreGame.h"
#include "OgreSceneManager.h"

class MyGame : public OgreGame {
public:
     MyGame();
     ~MyGame();
protected:
     void init();
     void update(float dt);
private:
     Ogre::SceneManager* m_sceneManager;
};


#endif /* MY_GAME_H */
