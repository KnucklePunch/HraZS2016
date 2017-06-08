//
// Created by Motko on 7. 12. 2016.
//

#ifndef PPGSO_ARTILLERYCANNON_H
#define PPGSO_ARTILLERYCANNON_H

#include <mesh.h>
#include "object.h"

class Artillery;

class ArtilleryCannon : public Object {
public:
    float chargeup_time = 0;
    float fire_delay = 0;
    void fire(Scene &scene, float dt);
    ArtilleryCannon(Artillery &arty);
    ~ArtilleryCannon();

    Artillery &parent;

    bool Update(Scene &scene, float dt) override;
    void Render(Scene &scene) override;
private:
    static MeshPtr mesh;
    static ShaderPtr shader;
    static TexturePtr texture;
};

typedef std::shared_ptr<ArtilleryCannon> CannonPtr;

#endif //PPGSO_ARTILLERYCANNON_H
