//
// Created by Motko on 9. 12. 2016.
//

#include <mesh.h>
#include <text_fragment2.h>
#include <text_vertex2.h>
#include "Text.h"
#include "../scene.h"

void Text::Render(Scene &scene) {
    shader->Use();

    // use camera
    shader->SetMatrix(scene.camera->projectionMatrix, "ProjectionMatrix");
    shader->SetMatrix(scene.camera->viewMatrix, "ViewMatrix");

    // render mesh
    shader->SetMatrix(modelMatrix, "ModelMatrix");
    shader->SetTexture(texture, "Texture");
    mesh->Render();
}

bool Text::Update(Scene &scene, float dt) {
    GenerateModelMatrix();
    return true;
}

Text::Text() {

    position[0] = -0.2f;

    if (!shader) shader = ShaderPtr(new Shader{text_vertex2, text_fragment2});
    if (!texture) texture = TexturePtr(new Texture{"score.rgb", 160, 30});
    if (!mesh) mesh = MeshPtr(new Mesh{shader, "score.obj"});
}

Text::~Text() {

}

// shared resources
MeshPtr Text::mesh;
ShaderPtr Text::shader;
TexturePtr Text::texture;