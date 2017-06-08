//
// Created by HORIZON on 14. 12. 2016.
//

#ifndef PPGSO_CUP_H
#define PPGSO_CUP_H

#include <texture.h>
#include <shader.h>
#include <mesh.h>

#include "object.h"

// Simple object representing the player
// Reads keyboard status and manipulates its own position
// On Update checks collisions with Asteroid objects in the scene
class Cup : public Object {
public:
    Cup();
    ~Cup();

    bool Update(Scene &scene, float dt) override;
    void Render(Scene &scene) override;


private:



    // Static resources (Shared between instances)
    static MeshPtr mesh;
    static ShaderPtr shader;
    static TexturePtr texture;
};
typedef std::shared_ptr< Cup > CupPtr;





#endif //PPGSO_CUP_H
