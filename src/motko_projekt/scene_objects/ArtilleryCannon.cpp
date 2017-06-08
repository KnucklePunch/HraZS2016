//
// Created by Motko on 7. 12. 2016.
//
#include <object_fragment2.h>
#include <object_vertex2.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ArtilleryCannon.h"
#include "../scene.h"
#include "ArtilleryShell.h"

void ArtilleryCannon::fire(Scene &scene,float dt) {

    ShellPtr shell = ShellPtr(new ArtilleryShell(10.0f + 75.0f * chargeup_time,parent.rotation[2],rotation[1],parent.modelMatrix * modelMatrix));
    scene.objects.push_back(shell);
    shell->Update(scene,dt);
    chargeup_time = 0;
    fire_delay = 1;

}


bool ArtilleryCannon::Update(Scene &scene, float dt) {

    fire_delay -= dt;

    //update cannon elevation
    if (scene.keyboard[GLFW_KEY_C]) {
        rotation[1] -= M_PI/2 * dt;
        if (rotation[1]  < -5.0f * M_PI / 180.0f)
            rotation[1] = float(-5.0f * M_PI / 180.0f);
    }

    if (scene.keyboard[GLFW_KEY_F]) {
        rotation[1] += M_PI/2 * dt;
        if (rotation[1]  > 75.0f * M_PI / 180.0f)
            rotation[1] = float(75.0f * M_PI / 180.0f);
    }

    if (scene.keyboard[GLFW_KEY_SPACE] && fire_delay <= 0) {
        chargeup_time += dt;
    } else {
        if (chargeup_time != 0) {
            fire(scene,dt);
        }
    }

    GenerateModelMatrix();
    return true;
}

void ArtilleryCannon::Render(Scene &scene) {

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
    shader->SetMatrix(parent.modelMatrix * modelMatrix, "ModelMatrix");
    shader->SetTexture(texture, "Texture");
    mesh->Render();
}

ArtilleryCannon::~ArtilleryCannon() {

}

ArtilleryCannon::ArtilleryCannon(Artillery &arty) : parent(arty) {

    position.x = 0.0f;
    position.y = 1.54857f;
    rotation[1] = 0.1f;
    scale *= 0.65f;

    if (!shader) shader = ShaderPtr(new Shader{object_vertex2, object_fragment2});
    if (!texture) texture = TexturePtr(new Texture{"GunBarrelTex.rgb", 1024, 1024});
    if (!mesh) mesh = MeshPtr(new Mesh{shader, "GunBarrel.obj"});
}


// shared resources
MeshPtr ArtilleryCannon::mesh;
ShaderPtr ArtilleryCannon::shader;
TexturePtr ArtilleryCannon::texture;




