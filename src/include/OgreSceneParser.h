#ifndef OGRE_SCENE_PARSER_H
#define OGRE_SCENE_PARSER_H

#include <OgreSceneManager.h>

bool parseScene (
     const char           *pFileName,
     Ogre::SceneManager   *pSceneManager,
     const char           *pSceneName
);

#endif /* OGRE_SCENE_PARSER_H */
