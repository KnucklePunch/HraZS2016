//
// Created by Motko on 7. 12. 2016.
//

#include <glm/gtc/matrix_transform.hpp>
#include <mesh.h>
#include <object_fragment2.h>
#include <object_vertex2.h>
#include "ArtilleryShell.h"
#include "../scene.h"
#include "explosion.h"
#include "Target.h"
#include <glm/gtx/matrix_decompose.hpp>

float sign(float x) {
    return float((x > 0) - (x < 0));
}

bool ArtilleryShell::Update(Scene &scene, float dt) {

    float old_speed = speed[1];

    //apply gravity
    this->speed[1] -= 9.81f * dt;

    //calculate air resistance - turbulent flow - F = rho * c * S * v^2, in direction opposite to travel
    glm::vec3 resistance = {
            -1 * sign(speed[0]) * speed[0] * speed[0],
            -1 * sign(speed[1]) * speed[1] * speed[1],
            -1 * sign(speed[2]) * speed[2] * speed[2]
    };

    //apply air density (rho = 29kg/m3)
    resistance = resistance * 0.029f;

    //apply resistance coefficient (c = 0.4 for highly aerodynamic projectiles)
    resistance = resistance * 0.4f;

    //apply shell diameter (S = pi * r^2, pi = 3.14 (higher accuracy not needed), r = 155mm)
    resistance = resistance * 3.14f * 0.155f * 0.155f;

    //apply shell weight (42kg for 155mm M107 shells)
    resistance = resistance * 42.0f;

    //apply resistance deceleration
    this->speed += resistance * dt;

    //apply movement to shell
    this->position += speed * dt;

    rotation[1] *= speed[1] / old_speed;


    //detect impact
    if (this->position[1] <= 0.0f) {

        this->position[1] = 0;

        auto exp = ExplosionPtr(new Explosion());
        exp->position = position;
        exp->scale *= 3.5f;
        scene.objects.push_back(exp);

        if (position[0] < -25.0f || position[0] > 25.0f || position[2] < -25.0f || position[2] > 25.0f) {
            if (scene.player != NULL) {
                scene.player->missed = true;
            }
        }

        for (auto i : scene.objects) {

            auto target = std::dynamic_pointer_cast<Target>(i);
            if (!target)
                continue;

            if (glm::distance(position, target->position) < 4.0f) {
                target->was_hit = true;
            }

        }

        return false;
    }

    GenerateModelMatrix();

    return true;
}

void ArtilleryShell::Render(Scene &scene) {

    shader->Use();

    // use camera
    shader->SetMatrix(scene.camera->projectionMatrix, "ProjectionMatrix");
    shader->SetMatrix(scene.camera->viewMatrix, "ViewMatrix");

    // set lights
    auto countloc = shader->GetUniformLocation("LightsCount");
    glUniform1i(countloc,Object::lights.size());

    std::vector<float> light_data;

    for (auto i = Object::lights.begin(); i != Object::lights.end(); ++i) {
        light_data.push_back((*(*i))[0]);
        light_data.push_back((*(*i))[1]);
        light_data.push_back((*(*i))[2]);
    }

    auto loc =  shader->GetUniformLocation("Lights");
    glUniform3fv(loc,Object::lights.size(),&light_data[0]);

    shader->SetVector(scene.camera->position,"Camera");

    // render mesh
    shader->SetMatrix(modelMatrix, "ModelMatrix");
    shader->SetTexture(texture, "Texture");
    mesh->Render();
}

ArtilleryShell::~ArtilleryShell() {

}

ArtilleryShell::ArtilleryShell(float starting_speed, float horizontal, float vertical, glm::mat4 modelMatrix) {

    this->scale *= 0.6;

    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(modelMatrix, scale, rotation, this->position, skew, perspective);

    glm::vec3 euler = glm::eulerAngles(rotation);

    this->rotation[1] = vertical;
    this->rotation[2] = horizontal;

    this->speed[0] = starting_speed * cosf(horizontal) * cosf(vertical);
    this->speed[1] = starting_speed * sinf(vertical);
    this->speed[2] = -starting_speed * sinf(horizontal) * cosf(vertical);

    this->moveRelative(3.0f* cosf(vertical),0,0);
    this->position[1] += 3.0f * sinf(vertical);

    glm::vec3 delta = {
            cos(vertical) * sin(horizontal),
            sin(vertical),
            cos(vertical) * cos(horizontal)
    };

    if (!shader) shader = ShaderPtr(new Shader{object_vertex2, object_fragment2});
    if (!texture) texture = TexturePtr(new Texture{"Shell.rgb", 1024, 1024});
    if (!mesh) mesh = MeshPtr(new Mesh{shader, "Shell.obj"});
}

// shared resources
MeshPtr ArtilleryShell::mesh;
ShaderPtr ArtilleryShell::shader;
TexturePtr ArtilleryShell::texture;





