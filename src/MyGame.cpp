#include "MyGame.h"
#include "OgreSceneParser.h"

#include <iostream>
#include <OgreEntity.h>

MyGame::MyGame() {
     std::cout << "MyGame()\n";
     m_isGuiOpen = false;
}

MyGame::~MyGame() {
     std::cout << "~MyGame()\n";
}

void MyGame::init() {
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

     mAudio->play("background");

     CEGUI::SchemeManager::getSingleton().createFromFile("OgreTray.scheme");
     CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");

     CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("OgreTrayImages/MouseArrow");

     m_guiWindow = (CEGUI::FrameWindow*)CEGUI::WindowManager::getSingleton().loadLayoutFromFile("Menu.layout");
     m_guiWindow->setDragMovingEnabled(false);
     m_guiWindow->setRollupEnabled(false);

     CEGUI::Window* resumeBtn = m_guiWindow->getChild("ResumeButton");
     CEGUI::Window* exitBtn = m_guiWindow->getChild("ExitButton");;

     resumeBtn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&MyGame::resumeBtnCallback, this));
     exitBtn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&MyGame::exitBtnCallback, this));

     resumeBtn->subscribeEvent(CEGUI::Window::EventMouseEntersSurface, CEGUI::Event::Subscriber(&MyGame::playButtonEnterSound, this));

     mPlayerNode = m_sceneManager->getSceneNode("PlayerNode");

     Ogre::SceneNode* node = m_sceneManager->getSceneNode("Hands");
     Ogre::Entity* entity = (Ogre::Entity*)node->getAttachedObject("Hands");
     mHandsAnimations = entity->getAnimationState("hands_idle");
     mHandsAnimations->setLoop(true);
     mHandsAnimations->setEnabled(true);
}

bool MyGame::playButtonEnterSound(const CEGUI::EventArgs& args) {
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

     if (mInput->wasButtonPressed(KC_ESC) || mInput->wasButtonPressed(JS_BUTTON_7)) {
          if (m_isGuiOpen)
               closeGUI();
          else
               openGUI();
     }

     if (!m_isGuiOpen) {

          float mouseX = mInput->getAxis(MOUSE_X);
          float mouseY = mInput->getAxis(MOUSE_Y);

          mPlayerNode->yaw(Ogre::Degree( -mouseX*dt*3 ), Ogre::Node::TS_WORLD);
          mPlayerNode->pitch(Ogre::Degree(-mouseY * dt*3));

          float jsAxis3 = mInput->getAxis(JS_AXIS_3);
          float jsAxis4 = mInput->getAxis(JS_AXIS_4);

          mPlayerNode->yaw(Ogre::Degree( -jsAxis3*dt*50.0), Ogre::Node::TS_WORLD);
          mPlayerNode->pitch(Ogre::Degree(-jsAxis4*dt*50.0));

          if (mInput->isButtonDown(KC_A)) {
               mPlayerNode->translate(-10*dt, 0, 0, Ogre::Node::TransformSpace::TS_LOCAL);
          }
          if (mInput->isButtonDown(KC_D)) {
               mPlayerNode->translate(10*dt, 0, 0, Ogre::Node::TransformSpace::TS_LOCAL);
          }

          if (mInput->isButtonDown(KC_W)) {
               mPlayerNode->translate(0, 0, -10*dt, Ogre::Node::TransformSpace::TS_LOCAL);
          }
          else if (mInput->isButtonDown(KC_S)) {
               mPlayerNode->translate(0, 0, 10*dt, Ogre::Node::TransformSpace::TS_LOCAL);
          }

          float horizontal = mInput->getAxis(JS_AXIS_0);
          float vertical = mInput->getAxis(JS_AXIS_1);

          mPlayerNode->translate(horizontal*dt*10, 0, vertical*dt*10, Ogre::Node::TransformSpace::TS_LOCAL);
     }

}
