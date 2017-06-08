#include "asteroid.h"


#include "object_fragment.h"
#include "object_vertex.h"

Asteroid::Asteroid() {


  // Set random scale speed and rotation
  scale *= 1.5f;

  rotation = glm::vec3(Rand(-PI, PI), Rand(-PI, PI), Rand(-PI, PI));


  // Initialize static resources if needed
  if (!shader) shader = ShaderPtr(new Shader{object_vertex, object_fragment});
  if (!texture) texture = TexturePtr(new Texture{"scene.rgb", 800, 400});
  if (!mesh) mesh = MeshPtr(new Mesh{shader, "scene2.obj"});
}

Asteroid::~Asteroid() {
}

bool Asteroid::Update(Scene &scene, float dt) {

  return true;
}

void Asteroid::Render(Scene &scene) {
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
MeshPtr Asteroid::mesh;
ShaderPtr Asteroid::shader;
TexturePtr Asteroid::texture;
