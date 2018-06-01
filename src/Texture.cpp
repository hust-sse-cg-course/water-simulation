#include "Texture.h"

Texture::Texture(std::string filepath, std::string type) :
    Texture(filepath, type, Texture::WrapType::Repeat)
{
}

Texture::Texture(std::string filepath, std::string type, WrapType wraptype) : mType(type), mPath(filepath) {
    glGenTextures(1, &mTex);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format;
        if (nrChannels == 1) {
            format = GL_RED;
        }
        else if (nrChannels == 3) {
            format = GL_RGB;
        }
        else if (nrChannels == 4) {
            format = GL_RGBA;
        }

        GLint wrap;
        switch (wraptype)
        {
        case Texture::Repeat:
            wrap = GL_REPEAT;
            break;
        case Texture::Mirrored_Repeat:
            wrap = GL_MIRRORED_REPEAT;
            break;
        case Texture::Clamp:
            wrap = GL_CLAMP_TO_EDGE;
            break;
        default:
            wrap = GL_REPEAT;
            break;
        }

        glBindTexture(GL_TEXTURE_2D, mTex);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);

    mWidth = width;
    mHeight = height;
}

Texture::Texture(int width, int height) : mWidth(width), mHeight(height) {
    glGenTextures(1, &mTex);

    glBindTexture(GL_TEXTURE_2D, mTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Remember using float format: RGBA16F
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, mWidth, mHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::drawTo(std::function<void()> callback) {
    if (mFBO == 0) {
        glGenFramebuffers(1, &mFBO);
    }
    if (mRBO == 0) {
        glGenRenderbuffers(1, &mRBO);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, mRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mWidth, mHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRBO);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTex, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Framebuffer not complete!" << std::endl;
    }

    int v[4];
    glGetIntegerv(GL_VIEWPORT, v);
    glViewport(0, 0, mWidth, mHeight);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    callback();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glViewport(v[0], v[1], v[2], v[3]);
}

void Texture::bind(unsigned int idx) {
    glActiveTexture(GL_TEXTURE0 + idx);
    glBindTexture(GL_TEXTURE_2D, mTex);
}