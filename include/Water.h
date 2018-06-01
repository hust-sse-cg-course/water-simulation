#ifndef __WATER_H__
#define __WATER_H__

#include "Plane.h"
#include "Texture.h"
#include "Shader.h"

class Water {
public:
    Water();
    void addDrop(float x, float y, float radius, float strength);
    void update(float delta_time = 0.0166);
    void updateNormals(float delta_time = 0.0166);
    Texture *getInfoTexture();

private:
    unsigned int framebuffer, depthbuffer;
    Plane quad;
    Texture *front, *back;
    Shader *drop_shader, *v_update_shader, *n_update_shader;
    void swapTexture();
};

#endif