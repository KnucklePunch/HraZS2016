//
// Created by Motko on 9. 12. 2016.
//

#include <object_fragment2.h>
#include <object_vertex2.h>
#include "Target.h"

void Target::Render(Scene &scene) {
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

bool Target::Update(Scene &scene, float dt) {

    age += dt;

    if (was_hit) {

        scene.score += __max(0,10 - int(age));

        scene.objects.push_back(TargetPtr(new Target()));

        return false;
    }

    GenerateModelMatrix();
    return true;
}

Target::Target() {

    position[0] = Rand(-25,25);
    position[2] = Rand(-25,25);

    if (!shader) shader = ShaderPtr(new Shader{object_vertex2, object_fragment2});
    if (!texture) texture = TexturePtr(new Texture{"Target.rgb", 256, 256});
    if (!mesh) mesh = MeshPtr(new Mesh{shader, "Target.obj"});
}

Target::~Target() {

}

// shared resources
MeshPtr Target::mesh;
ShaderPtr Target::shader;
TexturePtr Target::texture;






