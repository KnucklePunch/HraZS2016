#include "generator.h"
#include "asteroid.h"
#include "explosion.h"
#include "player.h"

#include "object_fragment.h"
#include "object_vertex.h"

bool Generator::Update(Scene &scene, float dt) {
  for ( auto obj : scene.objects ) {
    // Ignore self in scene
    if (obj.get() == this)
      continue;

    // We only need to collide with asteroids, ignore other objects
    auto asteroid = std::dynamic_pointer_cast<Player>(obj);
    if (!asteroid) continue;

    if (glm::distance(position, asteroid->position) < asteroid->scale.y) {
      // Explode
      auto explosion = ExplosionPtr(new Explosion{});
      explosion->position = position;
      explosion->scale = scale * 3.0f;
      scene.objects.push_back(explosion);
        asteroid->youDied();
      // Die
      return false;
    }
  }
  GenerateModelMatrix();
  return true;
}

void Generator::Render(Scene &scene) {
  shader->Use();

  // use camera
  shader->SetMatrix(scene.camera->projectionMatrix, "ProjectionMatrix");
  shader->SetMatrix(scene.camera->viewMatrix, "ViewMatrix");

  // render mesh
  shader->SetMatrix(modelMatrix, "ModelMatrix");
  shader->SetTexture(texture, "Texture");
  mesh->Render();
}

Generator::~Generator() {
}

Generator::Generator() {
  scale *= 2.0f;

  position.z = -6;
  // Initialize static resources if needed
  if (!shader) shader = ShaderPtr(new Shader{object_vertex, object_fragment});
  if (!texture) texture = TexturePtr(new Texture{"Cup.rgb", 512, 512});
  if (!mesh) mesh = MeshPtr(new Mesh{shader, "cup2.obj"});
}

Generator::Generator(int offset) {
  scale *= 2.0f;

  position.z = -6.5;
  switch (offset){
    case 0:
      position.x = -8;
      position.y = 8;
      break;
    case 1:
      position.x = 0;
          position.y = 8;
          break;
    case 2:
      position.x = 8;
          position.y = 8;
          break;
    case 3:
      position.x = 0;
          position.y = 0;
          break;
    case 4:
      position.x = -4;
          position.y = -8;
          break;
    case 5:
      position.x = 4;
          position.y = -8;
          break;
  }
  // Initialize static resources if needed
  if (!shader) shader = ShaderPtr(new Shader{object_vertex, object_fragment});
  if (!texture) texture = TexturePtr(new Texture{"Cup.rgb", 512, 512});
  if (!mesh) mesh = MeshPtr(new Mesh{shader, "cup2.obj"});
}

ShaderPtr Generator::shader;
MeshPtr Generator::mesh;
TexturePtr Generator::texture;