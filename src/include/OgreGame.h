#ifndef OGRE_GAME_H
#define OGRE_GAME_H

#include "InputHandler.h"
#include "AudioPlayer.h"
#include "PhysicsHandler.h"

#include <OgreRoot.h>
#include <OgreRenderWindow.h>

class OgreGame : public Ogre::FrameListener {
public:
     // Constructor / Destructor
     OgreGame();
     virtual ~OgreGame() = 0;

     void launch();
     bool frameRenderingQueued(const Ogre::FrameEvent&);
     void exit();

protected:
     virtual void init() = 0;
     virtual void update(float dt) = 0;

     Ogre::Root* m_root;
     Ogre::RenderWindow* m_window;

     InputHandler* mInput;
     AudioPlayer* mAudio;
     PhysicsHandler* mPhysics;

private:
     bool m_running;

     void initGUI();
};

#endif /* OGRE_GAME_G */
