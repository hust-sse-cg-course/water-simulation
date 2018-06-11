#include "Water.h"

Water::Water() {
	tex_waterFront = new Texture(256, 256);
    tex_waterBack = new Texture(256, 256);
	tex_caustic = new Texture(1024, 1024);
    drop_shader = new Shader("res/shader/texture_draw.vs", "res/shader/drop.fs");
    v_update_shader = new Shader("res/shader/texture_draw.vs", "res/shader/vertex_update.fs");
    n_update_shader = new Shader("res/shader/texture_draw.vs", "res/shader/normal_update.fs");
	caustic_update_shader = new Shader("res/shader/caustic.vs", "res/shader/caustic.fs");
    glGenFramebuffers(1, &fb_water);
	glGenFramebuffers(1, &fb_caustic);
}

void Water::swapTexture() {
    unsigned int id = tex_waterFront->mTex;
    tex_waterFront->mTex = tex_waterBack->mTex;
    tex_waterBack->mTex = id;
}

void Water::addDrop(float x, float y, float radius, float strength) {
    tex_waterBack->drawTo([&]() {
        tex_waterFront->bind();
        drop_shader->use();
        drop_shader->setVec2("center", x, y);
        drop_shader->setFloat("radius", radius);
        drop_shader->setFloat("strength", strength);
        drop_shader->setInt("texture", 0);

        quad.draw(drop_shader);
    });

    swapTexture();
}

void Water::update(float delta_time) {
    tex_waterBack->drawTo([&]() {
        tex_waterFront->bind();
        v_update_shader->use();
        v_update_shader->setVec2("delta", 1.0 / tex_waterFront->mWidth, 1.0 / tex_waterFront->mHeight);
        v_update_shader->setInt("texture", 0);

        quad.draw(v_update_shader);
    });

    swapTexture();
}
void Water::updateCaustic(glm::vec3 lightdir, Plane p){
	tex_caustic->drawTo([&]() {
		tex_waterFront->bind();
		caustic_update_shader->use();
		caustic_update_shader->setInt("water", 0);
		caustic_update_shader->setVec3("light", lightdir);
		p.draw(caustic_update_shader);
	});
}

Texture *Water::getInfoTexture() {
    return tex_waterFront;
}

Texture *Water::getCausticTexture() {
	return tex_caustic;
}

void Water::updateNormals(float delta_time) {
    tex_waterBack->drawTo([&]() {
        tex_waterFront->bind();
        n_update_shader->use();
        n_update_shader->setVec2("delta", 1.0 / tex_waterFront->mWidth, 1.0 / tex_waterFront->mHeight);
        n_update_shader->setInt("texture", 0);

        quad.draw(n_update_shader);
    });

    swapTexture();
}