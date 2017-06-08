//
// Created by Motko on 7. 12. 2016.
//

#ifndef PPGSO_ARTILLERYSHELL_H
#define PPGSO_ARTILLERYSHELL_H


#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "object.h"

class ArtilleryShell : public Object {
public:
    glm::vec3 speed;
    bool Update(Scene &scene, float dt) override;
    void Render(Scene &scene) override;

    ArtilleryShell(float starting_speed, float horizontal, float vertical, glm::mat4 modelMatrix);
    ~ArtilleryShell();
private:
    static MeshPtr mesh;
    static ShaderPtr shader;
    static TexturePtr texture;
};

typedef std::shared_ptr<ArtilleryShell> ShellPtr;


#endif //PPGSO_ARTILLERYSHELL_H
