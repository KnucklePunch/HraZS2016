#include "scene.h"
#include "projectile.h"

#include "object_vertex.h"
#include "object_fragment.h"

#include <GLFW/glfw3.h>

Projectile::Projectile(float y) {

    position.z = -10;
    position.y = y;

    orig_position = position;
    orig_rotation = rotation;
  // Initialize static resources if needed
  if (!shader) shader = ShaderPtr(new Shader{object_vertex, object_fragment});
  if (!texture) texture = TexturePtr(new Texture{"missile.rgb", 512, 512});
  if (!mesh) mesh = MeshPtr(new Mesh{shader, "arrow.obj"});
}

Projectile::~Projectile() {
}

bool Projectile::Update(Scene &scene, float dt) {

    if(scene.keyboard[GLFW_KEY_LEFT]) {
        position.x += 10 * dt;
        rotation.y -= PI/100.0f;
    } else if(scene.keyboard[GLFW_KEY_RIGHT]) {
        position.x -= 10 * dt;
        rotation.y += PI / 100.0f;
    }
        GenerateModelMatrix();
  return true;
}

void Projectile::Render(Scene &scene) {
  shader->Use();

  // use camera
  shader->SetMatrix(scene.camera->projectionMatrix, "ProjectionMatrix");
  shader->SetMatrix(scene.camera->viewMatrix, "ViewMatrix");

  // render mesh
  shader->SetMatrix(modelMatrix, "ModelMatrix");
  shader->SetTexture(texture, "Texture");
  mesh->Render();
}

void Projectile::Reset(){
    position = orig_position;
    rotation = orig_rotation;
}

ShaderPtr Projectile::shader;
MeshPtr Projectile::mesh;
TexturePtr Projectile::texture;


