//
// Created by HORIZON on 14. 12. 2016.
//

#include "cup.h"
#include "scene.h"
#include "object_fragment.h"
#include "object_vertex.h"

#include <GLFW/glfw3.h>

Cup::Cup() {
  // Reset fire delay

  // Set the rate of fire

  // Fire offset;


  // Scale the default model
  scale *= 3.0f;

  // Initialize static resources if needed
  if (!shader) shader = ShaderPtr(new Shader{object_vertex, object_fragment});
  if (!texture) texture = TexturePtr(new Texture{"wall_rgb.rgb", 256, 512});
  if (!mesh) mesh = MeshPtr(new Mesh{shader, "sphere.obj"});
}

Cup::~Cup() {
}

bool Cup::Update(Scene &scene, float dt) {

  return true;
}

void Cup::Render(Scene &scene) {
  shader->Use();

  // use camera
  shader->SetMatrix(scene.camera->projectionMatrix, "ProjectionMatrix");
  shader->SetMatrix(scene.camera->viewMatrix, "ViewMatrix");

  // render mesh
  shader->SetMatrix(modelMatrix, "ModelMatrix");
  shader->SetTexture(texture, "Texture");
  mesh->Render();
}



// shared resources
MeshPtr Cup::mesh;
ShaderPtr Cup::shader;
TexturePtr Cup::texture;