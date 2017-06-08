//
// Created by Motko on 9. 12. 2016.
//

#ifndef PPGSO_TARGET_H
#define PPGSO_TARGET_H


#include <texture.h>
#include "../scene.h"

class Target : public Object {

public:
    Target();
    ~Target();

    float age = 0;
    bool was_hit = false;

    bool Update(Scene &scene, float dt) override;
    void Render(Scene &scene) override;

private:
    static MeshPtr mesh;
    static ShaderPtr shader;
    static TexturePtr texture;

};

typedef std::shared_ptr<Target> TargetPtr;


#endif //PPGSO_TARGET_H
