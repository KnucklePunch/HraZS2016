//
// Created by Motko on 8. 12. 2016.
//

#include <object_fragment2.h>
#include <object_vertex2.h>
#include "Terrain.h"

void Terrain::Render(Scene &scene) {
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

bool Terrain::Update(Scene &scene, float dt) {
    GenerateModelMatrix();
    return true;
}

Terrain::~Terrain() {

}

Terrain::Terrain() {

    scale *= 2;

    if (!shader) shader = ShaderPtr(new Shader{object_vertex2, object_fragment2});
    if (!texture) texture = TexturePtr(new Texture{"dirt.rgb", 2560, 1080});
    if (!mesh) mesh = MeshPtr(new Mesh{shader, "Surface.obj"});

}

// shared resources
MeshPtr Terrain::mesh;
ShaderPtr Terrain::shader;
TexturePtr Terrain::texture;








