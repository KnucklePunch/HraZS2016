#ifndef _PPGSO_SCENE_H
#define _PPGSO_SCENE_H

#include <memory>
#include <map>
#include <list>
#include <src/motko_projekt/scene_objects/object.h>
#include <src/motko_projekt/scene_objects/Artillery.h>

#include "scene_objects/object.h"
#include "src/motko_projekt/camera/camera.h"

const unsigned int SIZE = 800;

// Simple object that contains all scene related data
// Object pointers are stored in a list of objects
// Keyboard and Mouse states are stored in a map and struct
class Scene {
  public:
    Scene();
    ~Scene();

    // Animate all objects in scene
    void Update(float time);
    // Render all objects in scene
    void Render();

    CameraPtr camera;
    ArtilleryPtr player;
    std::list< ObjectPtr > objects;
    std::map< int, int > keyboard;
    struct {
      double x, y;
      bool left, right;
    } mouse;
    int score = 0;
};
typedef std::shared_ptr< Scene > ScenePtr;

#endif // _PPGSO_SCENE_H
