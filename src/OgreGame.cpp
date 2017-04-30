#include "OgreGame.h"

#include "OISInputHandler.h"
#include "BassAudioPlayer.h"

#include <OgreRenderSystem.h>
#include <OgreLogManager.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

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
     delete mPhysics;
     delete m_root;
}

void OgreGame::launch() {
     if (m_running) return;
     m_running = true;
     m_window = m_root->initialise(true, "OgreGame v0.1");
     mInput = new OISInputHandler(m_window);
     mAudio = new BassAudioPlayer();
     mPhysics = new PhysicsHandler();
     initGUI();
     init();
     m_root->startRendering();
}

void OgreGame::initGUI() {
     // Setup renderer
     CEGUI::OgreRenderer::bootstrapSystem();

     // Setup resource locaitons
     Ogre::ResourceGroupManager& rgm = Ogre::ResourceGroupManager::getSingleton();
     rgm.addResourceLocation("assets/cegui/imagesets", "FileSystem", "Imagesets");
     rgm.addResourceLocation("assets/cegui/fonts", "FileSystem", "Fonts");
     rgm.addResourceLocation("assets/cegui/schemes", "FileSystem", "Schemes");
     rgm.addResourceLocation("assets/cegui/looknfeel", "FileSystem", "LookNFeel");
     rgm.addResourceLocation("assets/cegui/layouts", "FileSystem", "Layouts");

     CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
     CEGUI::Font::setDefaultResourceGroup("Fonts");
     CEGUI::Scheme::setDefaultResourceGroup("Schemes");
     CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
     CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
}

bool OgreGame::frameRenderingQueued(const Ogre::FrameEvent& evnt) {
     if (m_window->isClosed()) {
          return false;
     }
     mInput->update();
     CEGUI::System::getSingleton().injectTimePulse(evnt.timeSinceLastFrame);
     mPhysics->stepSimulation(evnt.timeSinceLastFrame);
     update(evnt.timeSinceLastFrame);
     return m_running;
}

void OgreGame::exit() {
     m_running = false;
}
