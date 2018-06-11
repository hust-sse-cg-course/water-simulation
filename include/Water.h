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
	void updateCaustic(glm::vec3 lightdir,Plane p);
    void updateNormals(float delta_time = 0.0166);
    Texture *getInfoTexture();
	Texture *getCausticTexture();
private:
    unsigned int fb_water, fb_caustic;
    Plane quad;
    Texture *tex_waterFront, *tex_waterBack, *tex_caustic;
    Shader *drop_shader, *v_update_shader, *n_update_shader, *caustic_update_shader;
    void swapTexture();
};

#endif