#include "OgreSceneParser.h"

#include <tinyxml.h>
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreAnimation.h>
#include <OgreAnimationState.h>
#include <OgreAnimationTrack.h>

Ogre::Vector3 parseVector3(
     TiXmlElement*            pNode
){
     float x=0, y=0, z=0;
     pNode->QueryFloatAttribute("x", &x);
     pNode->QueryFloatAttribute("y", &y);
     pNode->QueryFloatAttribute("z", &z);
     return Ogre::Vector3(x, y, z);
}

Ogre::Quaternion parseQuaternion(
     TiXmlElement*            pNode
){
	float a = 0, x = 0, y = 0, z = 0;
     pNode->QueryFloatAttribute("a", &a);
     pNode->QueryFloatAttribute("x", &x);
     pNode->QueryFloatAttribute("y", &y);
     pNode->QueryFloatAttribute("z", &z);
     return Ogre::Quaternion(Ogre::Degree(a), Ogre::Vector3(x, y, z));
}

bool parseNode(
     TiXmlElement*            pNode,
     Ogre::SceneNode*         pParent,
     Ogre::SceneManager*      pSceneManager
){

     //Create the Ogre::SceneNode
     Ogre::SceneNode* _Node;
     const char* nodeName = pNode->Attribute("name");
     if (nodeName != NULL) {
          _Node = pParent->createChildSceneNode(nodeName);
     }
     else {
          _Node = pParent->createChildSceneNode();
     }

     //Parse animations
     for (TiXmlElement* animation = pNode->FirstChildElement("animation");
     	animation != NULL;
     	animation = animation->NextSiblingElement("animation"))
	{
		const char* animName = animation->Attribute("name");
     	if (animName == NULL) {
     		return false;
     	}
     	float len = 1;
     	animation->QueryFloatAttribute("len", &len);
     	Ogre::Animation* _Animation = pSceneManager->createAnimation(animName, len);
     	const char* interp = animation->Attribute("interp");
     	if (interp == NULL) {
	     	_Animation->setInterpolationMode(Ogre::Animation::IM_SPLINE);
     	}
     	else {
     		int result = strcmp(interp, "linear");
     		if (result == 0) {
		     	_Animation->setInterpolationMode(Ogre::Animation::IM_LINEAR);
     		}
     		else {
		     	_Animation->setInterpolationMode(Ogre::Animation::IM_SPLINE);
     		}
     	}

     	_Animation->setRotationInterpolationMode(Ogre::Animation::RIM_SPHERICAL);

     	Ogre::NodeAnimationTrack* _Track = _Animation->createNodeTrack(1, _Node);
     	Ogre::TransformKeyFrame* _Keyframe;
     	for (TiXmlElement* keyframe = animation->FirstChildElement("keyframe");
     		keyframe != NULL;
     		keyframe = keyframe->NextSiblingElement("keyframe"))
		{
			float t = 0;
			keyframe->QueryFloatAttribute("t", &t);
			_Keyframe = _Track->createNodeKeyFrame(t);
			TiXmlElement* translate = keyframe->FirstChildElement("translate");
			if (translate != NULL) {
				Ogre::Vector3 vec = parseVector3(translate);
				_Keyframe->setTranslate(vec);
			}
			TiXmlElement* rotation = keyframe->FirstChildElement("rotate");
			if (rotation != NULL) {
				Ogre::Quaternion q = parseQuaternion(rotation);
				_Keyframe->setRotation(q);
			}
			TiXmlElement* scale = keyframe->FirstChildElement("scale");
			if (scale != NULL) {
				Ogre::Vector3 vec = parseVector3(scale);
				_Keyframe->setScale(vec);
			}

		}
		pSceneManager->createAnimationState(animName);
	}

     //Parse entities
     for (TiXmlElement* entity = pNode->FirstChildElement("entity");
          entity != NULL;
          entity = entity->NextSiblingElement("entity"))
     {
          const char* entityName = entity->Attribute("name");
          const char* meshFile = entity->Attribute("meshFile");
          if (meshFile != NULL) {
               Ogre::Entity* _Entity;
               if (entityName != NULL) {
                    _Entity = pSceneManager->createEntity(entityName, meshFile);
               }
               else {
                    _Entity = pSceneManager->createEntity(meshFile);
               }
               const char* materialName = entity->Attribute("material");
               if (materialName != NULL) {
                    _Entity->setMaterialName(materialName);
               }
               _Node->attachObject(_Entity);
          }
     }

     //Parse lights
     for (TiXmlElement* light = pNode->FirstChildElement("light");
          light != NULL;
          light = light->NextSiblingElement("light"))
     {
          Ogre::Light* _Light;
          const char* lightName = light->Attribute("name");
          if (lightName != NULL) {
               _Light = pSceneManager->createLight(lightName);
          }
          else {
               return false;
          }
          TiXmlElement* position = light->FirstChildElement("position");
          if (position != NULL) {
               Ogre::Vector3 vec = parseVector3(position);
               _Light->setPosition(vec);
          }
     }

     //Parse cameras
     for (TiXmlElement* camera = pNode->FirstChildElement("camera");
          camera != NULL;
          camera = camera->NextSiblingElement("camera"))
     {
          Ogre::Camera* _Camera;
          const char* cameraName = camera->Attribute("name");
          if (cameraName != NULL) {
               _Camera = pSceneManager->createCamera(cameraName);
          }
          else {
               return false;
          }
          TiXmlElement* clipping = camera->FirstChildElement("clipping");
          if (clipping != NULL) {
               float nearClipDistance = 0.1, farClipDistance=100;
               clipping->QueryFloatAttribute("nearClipDistance", &nearClipDistance);
               clipping->QueryFloatAttribute("farClipDistance", &farClipDistance);
               _Camera->setNearClipDistance(nearClipDistance);
               _Camera->setFarClipDistance(farClipDistance);
          }
          TiXmlElement* position = camera->FirstChildElement("position");
          if (position != NULL) {
               Ogre::Vector3 vec = parseVector3(position);
               _Camera->setPosition(vec);
          }
          TiXmlElement* lookAt = camera->FirstChildElement("lookAt");
          if (lookAt != NULL) {
               Ogre::Vector3 vec = parseVector3(lookAt);
               _Camera->lookAt(vec);
          }
          _Node->attachObject(_Camera);
     }

     //Parse translations
     for (TiXmlElement* translation = pNode->FirstChildElement("translate");
          translation;
          translation = translation->NextSiblingElement("translate"))
     {
          Ogre::Vector3 vec = parseVector3(translation);
          _Node->translate(vec);
     }

     //Parse rotations
     for (TiXmlElement* rotation = pNode->FirstChildElement("rotate");
          rotation != NULL;
          rotation = rotation->NextSiblingElement("rotate"))
     {
          Ogre::Quaternion q = parseQuaternion(rotation);
          _Node->rotate(q);
     }

     //Parse scale
     for (TiXmlElement* scale = pNode->FirstChildElement("scale");
          scale != NULL;
          scale = scale->NextSiblingElement("scale"))
     {
          Ogre::Vector3 vec = parseVector3(scale);
          _Node->scale(vec);
     }

     //Parse children
     for (TiXmlElement* child = pNode->FirstChildElement("node");
          child != NULL;
          child = child->NextSiblingElement("node"))
     {
          parseNode(child, _Node, pSceneManager);
     }

     return true;
}

bool parseResources(
     TiXmlElement*            pResources,
     const char*              pSceneName
){

     //Get the resource group mSceneManager
     Ogre::ResourceGroupManager& rgm = Ogre::ResourceGroupManager::getSingleton();

     //Parse paths
     TiXmlElement* paths = pResources->FirstChildElement("paths");
     if(paths != NULL) {
          for (TiXmlElement* path = paths->FirstChildElement("path");
               path != NULL;
               path = path->NextSiblingElement("path"))
          {
               rgm.addResourceLocation(path->Attribute("dir"), "FileSystem", pSceneName);
          }
     }

     //Parse meshes
     TiXmlElement* meshes = pResources->FirstChildElement("meshes");
     if (meshes != NULL) {
          for (TiXmlElement* mesh = meshes->FirstChildElement("mesh");
               mesh != NULL;
               mesh = mesh->NextSiblingElement("mesh"))
          {
               rgm.declareResource(mesh->Attribute("file"), "Mesh", pSceneName);
          }
     }

     //Parse materials
     TiXmlElement* materials = pResources->FirstChildElement("materials");
     if (materials != NULL) {
          for (TiXmlElement* material = materials->FirstChildElement("material");
               material != NULL;
               material = material->NextSiblingElement("material"))
          {
               rgm.declareResource(material->Attribute("file"), "Material", pSceneName);
          }
     }

     //Parse textures
     TiXmlElement* textures = pResources->FirstChildElement("textures");
     if (textures != NULL) {
          for (TiXmlElement* texture = textures->FirstChildElement("texture");
               texture != NULL;
               texture = texture->NextSiblingElement("texture"))
          {
               rgm.declareResource(texture->Attribute("file"), "Texture", pSceneName);
          }
     }

     rgm.initialiseResourceGroup(pSceneName);
     return true;
}

bool parseNodes(
     TiXmlElement*            pNodes,
     Ogre::SceneManager*      pSceneManager
){
     //Parse nodes
     for (TiXmlElement* node = pNodes->FirstChildElement("node");
          node != NULL;
          node = node->NextSiblingElement("node"))
     {
          if (!parseNode(node,
                    pSceneManager->getRootSceneNode(), pSceneManager)) {
               return false;
          }
     }
     return true;
}

bool parseScene (
     const char*              pFileName,
     Ogre::SceneManager*      pSceneManager,
     const char*              pSceneName
){
     std::cout << "Parsing scene: " << pFileName << "\n";
     TiXmlDocument doc(pFileName);
     if (!doc.LoadFile()) {
          return false;
     }

     TiXmlElement* scene = doc.RootElement();

     std::cout << "* Parsing resources...\n";
     //Parse resources
     TiXmlElement* resources = scene->FirstChildElement("resources");
     if (resources != NULL) {
          if (!parseResources(resources, pSceneName)) {
               return false;
          }
     }

     std::cout << "* Parsing nodes...\n";
     //Parse scene nodes
     TiXmlElement* nodes = scene->FirstChildElement("nodes");
     if (nodes != NULL) {
          if (!parseNodes(nodes, pSceneManager)) {
               return false;
          }
     }

     std::cout << "Scene parsed!\n";
     return true;
}
