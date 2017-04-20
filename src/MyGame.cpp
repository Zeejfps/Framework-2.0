#include "MyGame.h"
#include "OgreSceneParser.h"

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <iostream>
#include <OgreEntity.h>

MyGame::MyGame() {
     std::cout << "MyGame()\n";
     m_isGuiOpen = false;
     m_playMusic = true;
}

MyGame::~MyGame() {
     std::cout << "~MyGame()\n";
}

void MyGame::init() {

     setupPhysicsWorld();

     m_sceneManager = m_root->createSceneManager(Ogre::ST_GENERIC);
     parseScene("assets/scenes/SimpleScene.xml", m_sceneManager, "SimpleScene");
     Ogre::Camera* camera = m_sceneManager->getCamera("MainCamera");
     Ogre::Viewport* viewport = m_window->addViewport(camera);
     float w = Ogre::Real(viewport->getActualWidth());
     float h = Ogre::Real(viewport->getActualHeight());
     float r = w/h;
     camera->setAspectRatio(r);
     mAudio->loadStream("assets/sounds/Background.mp3", "background");
     mAudio->loadSample("assets/sounds/click.wav", "click");
     mAudio->loadSample("assets/sounds/load.wav", "load");
     mAudio->loadSample("assets/sounds/negative.wav", "negative");
     mAudio->loadSample("assets/sounds/misc_sound.wav", "popout");

     mAudio->play("background");

     CEGUI::SchemeManager::getSingleton().createFromFile("OgreTray.scheme");
     CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
     CEGUI::SchemeManager::getSingleton().createFromFile("SampleBrowser.scheme");
     CEGUI::SchemeManager::getSingleton().createFromFile("Generic.scheme");

     CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("OgreTrayImages/MouseArrow");

     m_guiWindow = (CEGUI::FrameWindow*)CEGUI::WindowManager::getSingleton().loadLayoutFromFile("Menu.layout");
     m_guiWindow->setDragMovingEnabled(false);
     m_guiWindow->setRollupEnabled(false);

     CEGUI::Window* resumeBtn = m_guiWindow->getChild("ResumeButton");
     CEGUI::Window* exitBtn = m_guiWindow->getChild("ExitButton");;

     resumeBtn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&MyGame::resumeBtnCallback, this));
     exitBtn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&MyGame::exitBtnCallback, this));

     CEGUI::ToggleButton* musicToggle = (CEGUI::ToggleButton*) m_guiWindow->getChild("SoundToggle");
     musicToggle->setSelected(true);
     musicToggle->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&MyGame::toggleBtnCallback, this));

     mPlayerNode = m_sceneManager->getSceneNode("PlayerNode");
     m_sceneManager->setSkyBox(true, "Skybox-Material");

     Ogre::SceneNode* node = m_sceneManager->getSceneNode("Hands");
     Ogre::Entity* entity = (Ogre::Entity*)node->getAttachedObject("Hands");
     mHandsAnimations = entity->getAnimationState("hands_idle");
     mHandsAnimations->setLoop(true);
     mHandsAnimations->setEnabled(true);

     mAnimations = new Ogre::AnimationState*[5];

     node = m_sceneManager->getSceneNode("Spring");
     entity = (Ogre::Entity*)node->getAttachedObject("Spring");
     mAnimations[0] = entity->getAnimationState("spring");
     mAnimations[0]->setLoop(false);
     mAnimations[0]->setEnabled(true);
     mAnimations[0]->setTimePosition(7);

     mAnimations[1] = m_sceneManager->getAnimationState("hood_open_anim");
     mAnimations[1]->setLoop(false);
     mAnimations[1]->setEnabled(false);

     mAnimations[2] = m_sceneManager->getAnimationState("rotate");
     mAnimations[2]->setLoop(true);
     mAnimations[2]->setEnabled(true);

     node = m_sceneManager->getSceneNode("Squirrel01");
     entity = (Ogre::Entity*)node->getAttachedObject("Squirrel01");
     mAnimations[3] = entity->getAnimationState("my_animation");
     mAnimations[3]->setLoop(true);
     mAnimations[3]->setEnabled(true);

     node = m_sceneManager->getSceneNode("Squirrel02");
     entity = (Ogre::Entity*)node->getAttachedObject("Squirrel02");
     mAnimations[4] = entity->getAnimationState("my_animation");
     mAnimations[4]->setLoop(true);
     mAnimations[4]->setEnabled(true);
}

bool MyGame::toggleBtnCallback(const CEGUI::EventArgs& args) {
     if (m_playMusic) {
          m_playMusic = false;
          mAudio->stop("background");
     }
     else {
          m_playMusic = true;
          mAudio->play("background");
     }
     return true;
}


bool MyGame::resumeBtnCallback(const CEGUI::EventArgs& args) {
     mAudio->play("click");
     closeGUI();
     return true;
}

bool MyGame::exitBtnCallback(const CEGUI::EventArgs& args) {
     mAudio->play("click");
     exit();
     return true;
}

void MyGame::closeGUI() {
     if (!m_isGuiOpen) return;
     m_isGuiOpen = false;
     mAudio->play("negative");
     CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(NULL);
     CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
}

void MyGame::openGUI() {
     if (m_isGuiOpen) return;
     m_isGuiOpen = true;
     mAudio->play("load");
     CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(m_guiWindow);
     CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
}

void MyGame::update(float dt) {

     mHandsAnimations->addTime(dt);

     for (int i = 0; i < 5; i++) {
          mAnimations[i]->addTime(dt);
     }

     if (mInput->wasButtonPressed(KC_ESC) || mInput->wasButtonPressed(JS_BUTTON_7)) {
          if (m_isGuiOpen)
               closeGUI();
          else
               openGUI();
     }

     if (!m_isGuiOpen) {

          if (mInput->wasButtonPressed(KC_SPACE) || mInput->wasButtonPressed(JS_BUTTON_0)) {
               mAudio->play("popout");
               mAnimations[0]->setTimePosition(0);
               mAnimations[1]->setTimePosition(0);
               mAnimations[0]->setEnabled(true);
               mAnimations[1]->setEnabled(true);
          }

          float mouseX = mInput->getAxis(MOUSE_X);
          float mouseY = mInput->getAxis(MOUSE_Y);

          mPlayerNode->yaw(Ogre::Degree( -mouseX*dt*3 ), Ogre::Node::TS_WORLD);
          mPlayerNode->pitch(Ogre::Degree(-mouseY * dt*3));

          float jsAxis3 = mInput->getAxis(JS_AXIS_3);
          float jsAxis4 = mInput->getAxis(JS_AXIS_4);

          mPlayerNode->yaw(Ogre::Degree( -jsAxis3*dt*50.0), Ogre::Node::TS_WORLD);
          mPlayerNode->pitch(Ogre::Degree(-jsAxis4*dt*50.0));

          if (mInput->isButtonDown(KC_A)) {
               mPlayerNode->translate(-10*dt, 0, 0, Ogre::Node::TS_LOCAL);
          }
          if (mInput->isButtonDown(KC_D)) {
               mPlayerNode->translate(10*dt, 0, 0, Ogre::Node::TS_LOCAL);
          }

          if (mInput->isButtonDown(KC_W)) {
               mPlayerNode->translate(0, 0, -10*dt, Ogre::Node::TS_LOCAL);
          }
          else if (mInput->isButtonDown(KC_S)) {
               mPlayerNode->translate(0, 0, 10*dt, Ogre::Node::TS_LOCAL);
          }

          float horizontal = mInput->getAxis(JS_AXIS_0);
          float vertical = mInput->getAxis(JS_AXIS_1);

          mPlayerNode->translate(horizontal*dt*10, 0, vertical*dt*10, Ogre::Node::TS_LOCAL);
     }

}

void MyGame::setupPhysicsWorld() {
     btCollisionConfiguration* config = new btDefaultCollisionConfiguration();
     btCollisionDispatcher* dispatcher = new btCollisionDispatcher(config);
}
