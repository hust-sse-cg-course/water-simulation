#include "Water.h"

Water::Water() {
    front = new Texture(256, 256);
    back = new Texture(256, 256);
    drop_shader = new Shader("res/shader/texture_draw.vs", "res/shader/drop.fs");
    v_update_shader = new Shader("res/shader/texture_draw.vs", "res/shader/vertex_update.fs");
    n_update_shader = new Shader("res/shader/texture_draw.vs", "res/shader/normal_update.fs");
    glGenFramebuffers(1, &framebuffer);
    glGenRenderbuffers(1, &depthbuffer);
}

void Water::swapTexture() {
    unsigned int id = front->mTex;
    front->mTex = back->mTex;
    back->mTex = id;
}

void Water::addDrop(float x, float y, float radius, float strength) {
    back->drawTo([&]() {
        front->bind();
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
    back->drawTo([&]() {
        front->bind();
        v_update_shader->use();
        v_update_shader->setVec2("delta", 1.0 / front->mWidth, 1.0 / front->mHeight);
        v_update_shader->setInt("texture", 0);

        quad.draw(v_update_shader);
    });

    swapTexture();
}

Texture *Water::getInfoTexture() {
    return front;
}

void Water::updateNormals(float delta_time) {
    back->drawTo([&]() {
        front->bind();
        n_update_shader->use();
        n_update_shader->setVec2("delta", 1.0 / front->mWidth, 1.0 / front->mHeight);
        n_update_shader->setInt("texture", 0);

        quad.draw(n_update_shader);
    });

    swapTexture();
}