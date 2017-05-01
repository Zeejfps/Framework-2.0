#ifndef MY_GAME_H
#define MY_GAME_H

#include "OgreGame.h"
#include "DebugDraw.hpp"

#include <OgreSceneManager.h>
#include <CEGUI/CEGUI.h>
#include <OgreSceneNode.h>
#include <OgreAnimationState.h>

#include "KinematicMotionState.h"

class MyGame : public OgreGame {
public:
     MyGame();
     ~MyGame();
protected:
     void init();
     void update(float dt);
private:
     Ogre::SceneManager* m_sceneManager;
     CEGUI::FrameWindow* m_guiWindow;
     bool m_isGuiOpen;
     Ogre::SceneNode* mPlayerNode;
     Ogre::SceneNode* mSphereNode;
     Ogre::SceneNode* mTruckNode;
     void openGUI();
     void closeGUI();
     bool resumeBtnCallback(const CEGUI::EventArgs&);
     bool exitBtnCallback(const CEGUI::EventArgs&);
     bool m_playMusic;
     bool toggleBtnCallback(const CEGUI::EventArgs&);
     Ogre::AnimationState *mHandsAnimations;
     Ogre::AnimationState **mAnimations;
     CDebugDraw* mDebugDrawer;
     KinematicMotionState* truckMotionState;
};


#endif /* MY_GAME_H */
