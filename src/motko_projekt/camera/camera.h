#ifndef PPGSO_CAMERA_H
#define PPGSO_CAMERA_H

#include <memory>

#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/detail/type_vec3.hpp>

// Simple camera object that keeps track of viewMatrix and projectionMatrix
// the projectionMatrix is by default constructed as perspective projection
// the viewMatrix is generated from up, position and back vectors on Update
class Camera {
public:
    Camera(float fow = 45.0f, float ratio = 1.0f, float near = 0.1f, float far = 10.0f);
    ~Camera();

    void Update(Scene& scene, float dt);
    Camera& moveRelative(float forward, float left, float up);
    Camera& rotate(float horizontal, float vertical);

    glm::vec3 up;
    glm::vec3 position;
    glm::vec3 back;

    float horizontal;
    float vertical;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
private:
    double last_cursor_x;
    double last_cursor_y;
    bool cursor_coord_initialized = false;
};
typedef std::shared_ptr< Camera > CameraPtr;

#endif //PPGSO_CAMERA_H
