#include "player.h"
#include "scene.h"
#include "asteroid.h"
#include "projectile.h"
#include "explosion.h"

#include "object_fragment.h"
#include "object_vertex.h"

#include <GLFW/glfw3.h>

Player::Player() {

    x = 0;
    y = -40;
    z = -6;

    died = false;

  // Scale the default model
  scale *= 3.0f;

    objekt = Sub_playerPtr(new Sub_player());

    direction = glm::vec3(0.0, 1.0, 0.0);
    power = 0;
    fired = false;

  // Initialize static resources if needed
  if (!shader) shader = ShaderPtr(new Shader{object_vertex, object_fragment});
  if (!texture) texture = TexturePtr(new Texture{"wall_rgb.rgb", 256, 512});
  if (!mesh) mesh = MeshPtr(new Mesh{shader, "sphere.obj"});
}

Player::~Player() {
}

bool Player::Update(Scene &scene, float dt) {
    if(died){
        return false;
    }

  if(!fired) {
      // Keyboard controls
      if (scene.keyboard[GLFW_KEY_LEFT]) {
          //position.x += 10 * dt;
          //rotation.z -= PI/100.0f;
          direction.x +=  dt;
          direction.y +=  dt;
          printf("Direction is going x: %f, y: %f\n",direction.x,direction.y);
      } else if (scene.keyboard[GLFW_KEY_RIGHT]) {
          //position.x -= 10 * dt;
          //rotation.z += PI/100.0f;
          direction.x -= dt;
          direction.y += dt;
          printf("Direction is going x: %f, y: %f\n",direction.x,direction.y);
      } else {
          //rotation.z = 0;
      }

      if (scene.keyboard[GLFW_KEY_SPACE]) {
          power += 0.05;
      }

      if (scene.keyboard[GLFW_KEY_F]) {
          fired = true;
      }
  } else{
      position += direction * power;
      scene.camera->position = position;
  }
    if(position.y >= 37){
        auto explosion = ExplosionPtr(new Explosion{});
        explosion->position = position;
        explosion->scale = scale * 3.0f;
        scene.objects.push_back(explosion);
        return false;
    }


    if(scene.keyboard[GLFW_KEY_UP]) {
        scene.camera->back += glm::vec3(0, 0.5, 0);
        scene.camera->position += glm::vec3(0, 0.5, 0);
    }
    if(scene.keyboard[GLFW_KEY_DOWN]) {
        scene.camera->back += glm::vec3(0, -0.5, 0);
        scene.camera->position += glm::vec3(0, -0.5, 0);
    }
    if(scene.keyboard[GLFW_KEY_W]){
        position.y += 0.5f;
    }
    if(scene.keyboard[GLFW_KEY_S]){
        position.y -= 0.5f;
    }

    objekt->Update(scene, dt, *this);

    GenerateModelMatrix();
  return true;
}

void Player::Render(Scene &scene) {
  shader->Use();

  // use camera
  shader->SetMatrix(scene.camera->projectionMatrix, "ProjectionMatrix");
  shader->SetMatrix(scene.camera->viewMatrix, "ViewMatrix");



  // render mesh
  shader->SetMatrix(modelMatrix, "ModelMatrix");
  shader->SetTexture(texture, "Texture");
  mesh->Render();
    objekt->Render(scene);
}

void Player::Reset() {
    position.x = x;
    position.y = y;
    position.z = z;
}

void Player::youDied() {
    died = true;
}

// shared resources
MeshPtr Player::mesh;
ShaderPtr Player::shader;
TexturePtr Player::texture;