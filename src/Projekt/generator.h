#ifndef PPGSO_GENERATOR_H
#define PPGSO_GENERATOR_H

#include <texture.h>
#include <shader.h>
#include <mesh.h>

#include <random>
#include "object.h"
#include "scene.h"

// Example generator of objects
// Constructs a new object during Update and adds it into the scene
// Does not render anything
class Generator : public Object {
public:
  Generator();
    Generator(int offset);
  ~Generator();

  bool Update(Scene &scene, float dt) override;
  void Render(Scene &scene) override;

    static MeshPtr mesh;
    static ShaderPtr shader;
    static TexturePtr texture;
};
typedef std::shared_ptr< Generator > GeneratorPtr;

#endif //PPGSO_GENERATOR_H
