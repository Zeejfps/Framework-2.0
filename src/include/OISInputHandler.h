#ifndef OIS_INPUT_HANDLER_H
#define OIS_INPUT_HANDLER_H

#include "InputHandler.h"

#include <map>
#include <OISMouse.h>
#include <OISKeyboard.h>
#include <OISJoyStick.h>
#include <OISInputManager.h>
#include <OgreRenderWindow.h>
#include <OgreWindowEventUtilities.h>

class OISInputHandler : public InputHandler, public OIS::KeyListener, public OIS::MouseListener, public OIS::JoyStickListener, public Ogre::WindowEventListener {
public:
     OISInputHandler(Ogre::RenderWindow*);
     ~OISInputHandler();
     void update();
     float getAxis(Axis);
     bool isButtonDown(Button);
     bool wasButtonPressed(Button);
     bool wasButtonReleased(Button);
     bool keyPressed(const OIS::KeyEvent&);
     bool keyReleased(const OIS::KeyEvent&);
     bool mouseMoved(const OIS::MouseEvent&);
     bool mousePressed(const OIS::MouseEvent&, OIS::MouseButtonID);
     bool mouseReleased(const OIS::MouseEvent&, OIS::MouseButtonID);
     void windowResized(Ogre::RenderWindow* rw);
     void windowClosed(Ogre::RenderWindow* rw);
     bool axisMoved(const OIS::JoyStickEvent&, int);
     bool povMoved( const OIS::JoyStickEvent &, int);
     bool buttonPressed(const OIS::JoyStickEvent&, int);
     bool buttonReleased(const OIS::JoyStickEvent&, int);
private:
     OIS::InputManager* mInputManager;
     OIS::Keyboard* mKeyboard;
     OIS::Mouse* mMouse;
     OIS::JoyStick* mJoyStick;
     int mWindowHnd;
     void free();
     Button mapKeyToButton(OIS::KeyCode);
     OIS::KeyCode mapButtonToKey(Button);
     std::map<Button, bool> mKeysPressed;
     std::map<Button, bool> mKeysReleased;
     std::map<Axis, float> mAxes;
};

#endif /* OIS_INPUT_HANDLER_H */
