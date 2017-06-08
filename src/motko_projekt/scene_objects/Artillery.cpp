//
// Created by Motko on 6. 12. 2016.
//

#include <object_fragment2.h>
#include <object_vertex2.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Artillery.h"
#include "../scene.h"
#include "Text.h"
#include "Number.h"

void displayNumber(Scene &scene, int number, float delta) {

    int number2 = number / 10;

    if (number2 != 0)
        displayNumber(scene,number2,delta - 0.24f);

    auto n = NumberPtr(new Number(number % 10));
    n->position[0] = delta;
    scene.objects.push_back(n);
}

void Artillery::Render(Scene &scene) {

    artillery_cannon->Render(scene);

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

bool Artillery::Update(Scene &scene, float dt) {

    if (position[0] < -25.0f || position[0] > 25.0f || position[2] < -25.0f || position[2] > 25.0f) {
        if (scene.player != NULL) {
            scene.player->missed = true;
        }
    }

    if (missed) {

        scene.player = NULL;

        TextPtr score = TextPtr(new Text());
        scene.objects.push_back(score);
        displayNumber(scene,scene.score,0.9f);

        return false;
    }

    if (scene.keyboard[GLFW_KEY_UP]) {
        moveRelative(8.0f * dt,0,0);
    }

    if (scene.keyboard[GLFW_KEY_DOWN]) {
        moveRelative(-8.0f * dt,0,0);
    }

    if (scene.keyboard[GLFW_KEY_LEFT]) {
        rotation[2] += M_PI/2 * dt;
    }

    if (scene.keyboard[GLFW_KEY_RIGHT]) {
        rotation[2] -= M_PI/2 * dt;
    }

    GenerateModelMatrix();

    artillery_cannon->Update(scene,dt);
    return true;
}

Artillery::Artillery() {

    artillery_cannon = CannonPtr(new ArtilleryCannon(*this));

    if (!shader) shader = ShaderPtr(new Shader{object_vertex2, object_fragment2});
    if (!texture) texture = TexturePtr(new Texture{"Chassis.rgb", 1024, 1024});
    if (!mesh) mesh = MeshPtr(new Mesh{shader, "Chassis.obj"});
}

Artillery::~Artillery() {

}

// shared resources
MeshPtr Artillery::mesh;
ShaderPtr Artillery::shader;
TexturePtr Artillery::texture;






