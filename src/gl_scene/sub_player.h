//
// Created by HORIZON on 14. 12. 2016.
//

#ifndef PPGSO_SUB_PLAYER_H
#define PPGSO_SUB_PLAYER_H


#include <mesh.h>
#include <shader.h>
#include <texture.h>
#include "object.h"

class Sub_player : public Object {
public:
    Sub_player();
    ~Sub_player();


    bool Update(Scene &scene, float dt) override;
    void Render(Scene &scene) override;

    static MeshPtr mesh;
    static ShaderPtr shader;
    static TexturePtr texture;
};
typedef std::shared_ptr< Sub_player > Sub_playerPtr;

#endif //PPGSO_SUB_PLAYER_H
