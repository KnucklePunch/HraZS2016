//
// Created by Motko on 6. 12. 2016.
//

#ifndef PPGSO_ARTILLERY_H
#define PPGSO_ARTILLERY_H


#include <texture.h>
#include <shader.h>
#include <mesh.h>
#include "object.h"
#include "ArtilleryCannon.h"

class Artillery : public Object {
public:
    Artillery();
    ~Artillery();

    bool missed = false;

    CannonPtr artillery_cannon;

    bool Update(Scene &scene, float dt);
    void Render(Scene &scene);
private:
    static MeshPtr mesh;
    static ShaderPtr shader;
    static TexturePtr texture;
};

typedef std::shared_ptr<Artillery> ArtilleryPtr;


#endif //PPGSO_ARTILLERY_H
