#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

class ResourceManager {
public:
     virtual void declarePath(const char* path, const char* group) = 0;
     virtual void declareMaterial(const char* material, const char* group) = 0;
     virtual void declareMesh(const char* mesh, const char* group) = 0;
     virtual void declareSound(const char* sound, const char* group) = 0;
};

#endif
