//
// Created by HORIZON on 14. 12. 2016.
//

#include "scene.h"
#include "sub_player.h"


#include "object_fragment.h"
#include "object_vertex.h"

Sub_player::Sub_player() {
    scale *= 0.5f;

    position.z = -6;
    // Initialize static resources if needed
    if (!shader) shader = ShaderPtr(new Shader{object_vertex, object_fragment});
    if (!texture) texture = TexturePtr(new Texture{"lena.rgb", 256, 512});
    if (!mesh) mesh = MeshPtr(new Mesh{shader, "sphere.obj"});
}

Sub_player::~Sub_player() {}

void Sub_player::Render(Scene &scene) {
    shader->Use();

    // use camera
    shader->SetMatrix(scene.camera->projectionMatrix, "ProjectionMatrix");
    shader->SetMatrix(scene.camera->viewMatrix, "ViewMatrix");

    // render mesh
    shader->SetMatrix(modelMatrix, "ModelMatrix");
    shader->SetTexture(texture, "Texture");
    mesh->Render();
}

bool Sub_player::Update(Scene &scene, float dt) {
    return true;
}

bool Sub_player::Update(Scene &scene, float dt, Object &player) {
    position = glm::vec3(0.0,-4.0,0.0) +  player.position;
    GenerateModelMatrix();
    return true;
}

// shared resources
MeshPtr Sub_player::mesh;
ShaderPtr Sub_player::shader;
TexturePtr Sub_player::texture;