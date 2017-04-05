#include "MyGame.h"
#include "OgreSceneParser.h"

#include <iostream>

MyGame::MyGame() {
     std::cout << "MyGame()\n";
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
     mAudio->loadSample("assets/sounds/door-close.wav", "door-close");
}

void MyGame::update(float dt) {
     if (mInput->wasButtonPressed(KC_ESC)) {
          exit();
     }

     if (mInput->wasButtonPressed(KC_SPACE)) {
          mAudio->playSample("door-close");
     }
}
