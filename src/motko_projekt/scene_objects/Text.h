//
// Created by Motko on 9. 12. 2016.
//

#ifndef PPGSO_TEXT_H
#define PPGSO_TEXT_H


#include "object.h"

class Text : public Object {

public:
    Text();
    ~Text();

    bool Update(Scene &scene, float dt) override;
    void Render(Scene &scene) override;
private:
    static MeshPtr mesh;
    static ShaderPtr shader;
    static TexturePtr texture;

};

typedef std::shared_ptr<Text> TextPtr;

#endif //PPGSO_TEXT_H
