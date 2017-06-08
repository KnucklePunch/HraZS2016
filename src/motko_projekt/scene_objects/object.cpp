#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "object.h"

using namespace glm;

Object::Object() {
  position = glm::vec3(0,0,0);
  scale = glm::vec3(1,1,1);
  rotation = glm::vec3(0,0,0);
  modelMatrix = glm::mat4(1.0f);
}

Object::~Object() {
}

void Object::GenerateModelMatrix() {
  modelMatrix =
          glm::translate(glm::mat4(1.0f), position)
          * glm::orientate4(rotation)
          * glm::scale(glm::mat4(1.0f), scale);
}

float Object::Rand(float min, float max) {
  return ((max - min) * ((float) rand() / (float) RAND_MAX)) + min;
}

void Object::moveRelative(float forward, float left, float up) {

  vec3 delta = {
          cos(rotation[1]) * sin(rotation[2]),
          sin(rotation[1]),
          cos(rotation[1]) * cos(rotation[2])
  };

  vec4 vector = {
          -forward,
          up,
          left,
          0
  };

  vec4 result = vector * lookAt(position, position + delta, vec3{0,1,0});
  position[0] += result[0];
  position[1] += result[1];
  position[2] += result[2];
}

std::vector<glm::vec3*> Object::lights;