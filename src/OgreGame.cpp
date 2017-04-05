#include "OgreGame.h"

#include "OISInputHandler.h"
#include "BassAudioPlayer.h"

#include <OgreRenderSystem.h>
#include <OgreLogManager.h>

OgreGame::OgreGame() : m_running(false) {
     m_root = new Ogre::Root("", "");
     m_root->loadPlugin("RenderSystem_GL");
     m_root->addFrameListener(this);

     Ogre::RenderSystem* rs = m_root->getRenderSystemByName("OpenGL Rendering Subsystem");
     m_root->setRenderSystem(rs);
     rs->setConfigOption("Full Screen", "No");
     rs->setConfigOption("Video Mode", "800 x 600 @ 32-bit colour");
}

OgreGame::~OgreGame() {
     delete mInput;
     delete mAudio;
     delete m_root;
}

void OgreGame::launch() {
     if (m_running) return;
     m_running = true;
     m_window = m_root->initialise(true, "OgreGame v0.1");
     mInput = new OISInputHandler(m_window);
     mAudio = new BassAudioPlayer();
     init();
     m_root->startRendering();
}

bool OgreGame::frameRenderingQueued(const Ogre::FrameEvent& evnt) {
     if (m_window->isClosed()) {
          return false;
     }
     mInput->update();
     update(evnt.timeSinceLastFrame);
     return m_running;
}

void OgreGame::exit() {
     m_running = false;
}
