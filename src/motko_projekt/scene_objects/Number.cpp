//
// Created by Motko on 9. 12. 2016.
//

#include "Number.h"
//
// Created by Motko on 9. 12. 2016.
//

#include <mesh.h>
#include <text_fragment2.h>
#include <text_vertex2.h>
#include "Text.h"
#include "../scene.h"

void Number::Render(Scene &scene) {
    shader->Use();

    // use camera
    shader->SetMatrix(scene.camera->projectionMatrix, "ProjectionMatrix");
    shader->SetMatrix(scene.camera->viewMatrix, "ViewMatrix");

    // render mesh
    shader->SetMatrix(modelMatrix, "ModelMatrix");
    shader->SetTexture(texture[number], "Texture");
    mesh->Render();
}

bool Number::Update(Scene &scene, float dt) {
    GenerateModelMatrix();
    return true;
}

Number::Number(int number) {

    this->number = number;

    if (!shader) shader = ShaderPtr(new Shader{text_vertex2, text_fragment2});
    if (!texture[0]) {
        for (int i = 0; i < 10; ++i) {
            char str[7];
            sprintf(str,"%d.data",i);
            texture[i] = TexturePtr(new Texture(str,24,30));
        }
    }
    if (!mesh) mesh = MeshPtr(new Mesh{shader, "number.obj"});
}

Number::~Number() {

}

// shared resources
MeshPtr Number::mesh;
ShaderPtr Number::shader;
TexturePtr Number::texture[10];