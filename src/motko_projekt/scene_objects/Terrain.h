//
// Created by Motko on 8. 12. 2016.
//

#ifndef PPGSO_TERRAIN_H
#define PPGSO_TERRAIN_H


#include <src/motko_projekt/scene.h>

class Terrain : public Object {
public:
    Terrain();
    ~Terrain();

    bool Update(Scene &scene, float dt) override;
    void Render(Scene &scene) override;
private:
    static MeshPtr mesh;
    static ShaderPtr shader;
    static TexturePtr texture;
};

typedef std::shared_ptr<Terrain> TerrainPtr;


#endif //PPGSO_TERRAIN_H
