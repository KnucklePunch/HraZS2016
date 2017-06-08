#include <glm/gtc/matrix_transform.hpp>
#include <src/motko_projekt/scene.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "camera.h"

#define PI 3.14159265358979323846f

using namespace glm;

Camera::Camera(float fow, float ratio, float near, float far) {
  float fowInRad = (PI/180.0f) * fow;

  projectionMatrix = glm::perspective(fowInRad, ratio, near, far);

  up = glm::vec3(0,1,0);
  position = glm::vec3(0,3,0);
  back = glm::vec3(0,0,-1);

  vertical = 0;
  horizontal = 0;
}

Camera::~Camera() {
}

Camera& Camera::moveRelative(float forward, float left, float up) {
  if (forward == 0 && left == 0 && up == 0) {
    return *this;
  }

  vec3 delta = {
          cos(vertical) * sin(horizontal),
          sin(vertical),
          cos(vertical) * cos(horizontal)
  };

  vec4 vector = {
          up,
          left,
          -forward,
          0
  };

  vec4 result = vector * lookAt(position, position + delta, vec3{0,1,0});
  position[0] += result[0];
  position[1] += result[1];
  position[2] += result[2];

  return *this;
}

Camera& Camera::rotate(float horizontal, float vertical) {
  if (horizontal == 0 && vertical == 0)
    return *this;

  this->horizontal += horizontal;
  this->vertical += vertical;

  return *this;
}

void Camera::Update(Scene& scene, float dt) {

  //scale view change speed
  dt *= 4;

  if (cursor_coord_initialized) {
    this->rotate(float(-M_PI/12 * 0.003f * (scene.mouse.x - last_cursor_x)), float(-M_PI/12 * 0.003f * (scene.mouse.y - last_cursor_y)));
  } else {
    cursor_coord_initialized = true;
  }

  last_cursor_x = scene.mouse.x;
  last_cursor_y = scene.mouse.y;

  if (scene.keyboard[GLFW_KEY_W]) {
    this->moveRelative(1.5f * dt,0,0);
  }

  if (scene.keyboard[GLFW_KEY_S]) {
    this->moveRelative(-1.5f * dt,0,0);
  }

  if (scene.keyboard[GLFW_KEY_A]) {
    this->moveRelative(0,0,-1.5f * dt);
  }

  if (scene.keyboard[GLFW_KEY_D]) {
    this->moveRelative(0,0,1.5f * dt);
  }

  if (scene.keyboard[GLFW_KEY_LEFT_SHIFT]) {
    this->moveRelative(0,1.5f * dt,0);
  }

  if (scene.keyboard[GLFW_KEY_LEFT_CONTROL]) {
    this->moveRelative(0,-1.5f * dt,0);
  }

  vec3 delta = {
          cos(vertical) * sin(horizontal),
          sin(vertical),
          cos(vertical) * cos(horizontal)
  };

  viewMatrix = glm::lookAt(position, position + delta, up);
}
