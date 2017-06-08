//
// Created by Motko on 9. 12. 2016.
//

#ifndef PPGSO_NUMBER_H
#define PPGSO_NUMBER_H


#include <mesh.h>
#include <src/projekt/scene.h>

class Number : public Object {

public:

    int number;

    Number(int number);
    ~Number();

    bool Update(Scene &scene, float dt) override;
    void Render(Scene &scene) override;
private:
    static MeshPtr mesh;
    static ShaderPtr shader;
    static TexturePtr texture[10];

};

typedef std::shared_ptr<Number> NumberPtr;

#endif //PPGSO_NUMBER_H
