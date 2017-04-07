#include <iostream>
#include <sstream>
#include <OISInputHandler.h>

OISInputHandler::OISInputHandler(Ogre::RenderWindow* pWindow)
: mKeyboard(NULL), mMouse(NULL), mJoyStick(NULL)
{
     OIS::ParamList pl;
     std::ostringstream windowHndStr;

     pWindow->getCustomAttribute("WINDOW", &mWindowHnd);
     windowHndStr << mWindowHnd;
     pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

     mInputManager = OIS::InputManager::createInputSystem(pl);

     mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
     mKeyboard->setEventCallback(this);

     mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));
     mMouse->setEventCallback(this);

     if (mInputManager->getNumberOfDevices(OIS::OISJoyStick) > 0) {
          mJoyStick = static_cast<OIS::JoyStick*>(mInputManager->createInputObject(OIS::OISJoyStick, true));
          mJoyStick->setEventCallback(this);
     }

     Ogre::WindowEventUtilities::addWindowEventListener(pWindow, this);
     windowResized(pWindow);
}

OISInputHandler::~OISInputHandler() {
     if(mInputManager) {
          mInputManager->destroyInputObject(mMouse);
          mInputManager->destroyInputObject(mKeyboard);
          mInputManager->destroyInputObject(mJoyStick);
          OIS::InputManager::destroyInputSystem(mInputManager);
          mInputManager = NULL;
     }
}

bool OISInputHandler::keyPressed(const OIS::KeyEvent &evt) {
     CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
     if (context.injectKeyDown((CEGUI::Key::Scan)evt.key)) return true;
     if (context.injectChar((CEGUI::Key::Scan)evt.text)) return true;

     Button button = mapKeyToButton(evt.key);
     mKeysPressed[button] = true;
     return true;
}

bool OISInputHandler::keyReleased(const OIS::KeyEvent &evt) {
     if (CEGUI::System::getSingleton().getDefaultGUIContext()
          .injectKeyUp((CEGUI::Key::Scan)evt.key)) return true;
     Button button = mapKeyToButton(evt.key);
     mKeysReleased[button] = true;
     return true;
}

void OISInputHandler::update() {
     std::map<Button, bool>::iterator it;

     for (it = mKeysPressed.begin(); it != mKeysPressed.end(); it++) {
          mKeysPressed[it->first]=false;
     }
     for (it = mKeysReleased.begin(); it != mKeysReleased.end(); it++) {
          mKeysReleased[it->first]=false;
     }

     mAxes[MOUSE_X] = 0;
     mAxes[MOUSE_Y] = 0;
     mKeyboard->capture();
     mMouse->capture();
     if (mJoyStick != NULL) {
          mJoyStick->capture();
     }

}

float OISInputHandler::getAxis(Axis axis) {
     return mAxes[axis];
}

bool OISInputHandler::isButtonDown(Button button) {
     OIS::KeyCode code = mapButtonToKey(button);
     return mKeyboard->isKeyDown(code);
}

bool OISInputHandler::wasButtonPressed(Button button) {
     return mKeysPressed[button];
}

bool OISInputHandler::wasButtonReleased(Button button) {
     return mKeysReleased[button];
}

bool OISInputHandler::mouseMoved(const OIS::MouseEvent &evt) {
     OIS::MouseState state = evt.state;
     mAxes[MOUSE_X] = state.X.rel;
     mAxes[MOUSE_Y] = state.Y.rel;

     CEGUI::GUIContext &ctx =  CEGUI::System::getSingleton().getDefaultGUIContext();
     ctx.injectMouseMove(state.X.rel, state.Y.rel);
     // Scroll wheel.
     if (state.Z.rel)
          ctx.injectMouseWheelChange(state.Z.rel / 120.0f);
     return true;
}

CEGUI::MouseButton OISInputHandler::convertButton(OIS::MouseButtonID id) {
     switch (id) {
          case OIS::MB_Left:
               return CEGUI::LeftButton;
               break;

          case OIS::MB_Right:
               return CEGUI::RightButton;
               break;

          case OIS::MB_Middle:
               return CEGUI::MiddleButton;
               break;

          default:
               return CEGUI::LeftButton;
               break;
     }
}

bool OISInputHandler::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id) {
     CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id));
     return true;
}

bool OISInputHandler::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id) {
     CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id));
     return true;
}

void OISInputHandler::windowResized(Ogre::RenderWindow* rw) {
     unsigned int width, height, depth;
     int left, top;
     rw->getMetrics(width, height, depth, left, top);

     const OIS::MouseState &ms = mMouse->getMouseState();
     std::cout << width << ", " << height << "\n";
     ms.width = width;
     ms.height = height;
}

void OISInputHandler::windowClosed(Ogre::RenderWindow* rw) {}

bool OISInputHandler::povMoved(const OIS::JoyStickEvent &evt, int id) {
     // D_PAD
     return true;
}

bool OISInputHandler::axisMoved(const OIS::JoyStickEvent &evt, int id) {
     int value = evt.state.mAxes[id].abs;

     if (id == 0) {
          if ( value > -4000 && value < 4000) {
               mAxes[JS_AXIS_0] = 0;
          } else {
               float horizontal =  value / 32767.0;
               mAxes[JS_AXIS_0] = horizontal;
          }
     }
     else if (id == 1) {
          if ( value > -4000 && value < 4000) {
               mAxes[JS_AXIS_1] = 0;
          } else {
               float vertical = value / 32767.0;
               mAxes[JS_AXIS_1] = vertical;
          }
     }
     else if (id == 3) {
          if ( value > -4000 && value < 4000) {
               mAxes[JS_AXIS_3] = 0;
          } else {
               float x = value / 32767.0;
               mAxes[JS_AXIS_3] = x;
          }
     }
     else if (id == 4) {
          if ( value > -4000 && value < 4000) {
               mAxes[JS_AXIS_4] = 0;
          } else {
               float y = value / 32767.0;
               mAxes[JS_AXIS_4] = y;
          }
     }
     return true;
}

bool OISInputHandler::buttonPressed(const OIS::JoyStickEvent &evt, int button) {
     std::cout << button << "\n";
     switch (button) {
          case 0:
               mKeysPressed[JS_BUTTON_0] = true;
               break;
          case 7:
               mKeysPressed[JS_BUTTON_7] = true;
               break;
     }
     return true;
}

bool OISInputHandler::buttonReleased(const OIS::JoyStickEvent &evt, int button) {
     switch (button) {
          case 0:
               mKeysReleased[JS_BUTTON_0] = true;
               break;
          case 7:
               mKeysReleased[JS_BUTTON_7] = true;
               break;
     }
     return true;
}

OIS::KeyCode OISInputHandler::mapButtonToKey(Button button) {
     switch (button) {
          case KC_ESC:
               return OIS::KC_ESCAPE;
          case KC_A:
               return OIS::KC_A;
          case KC_S:
               return OIS::KC_S;
          case KC_D:
               return OIS::KC_D;
          case KC_W:
               return OIS::KC_W;
          case KC_E:
               return OIS::KC_E;
          case KC_SPACE:
               return OIS::KC_SPACE;
          case KC_ENTER:
               return OIS::KC_RETURN;
          default:
               return OIS::KC_UNASSIGNED;
     }
}

Button OISInputHandler::mapKeyToButton(OIS::KeyCode key) {
     switch (key) {
          case OIS::KC_ESCAPE:
               return KC_ESC;
          case OIS::KC_A:
               return KC_A;
          case OIS::KC_S:
               return KC_S;
          case OIS::KC_D:
               return KC_D;
          case OIS::KC_W:
               return KC_W;
          case OIS::KC_E:
               return KC_E;
          case OIS::KC_SPACE:
               return KC_SPACE;
          case OIS::KC_RETURN:
               return KC_ENTER;
          default:
               return KC_UNASSIGNED;
     }
}
