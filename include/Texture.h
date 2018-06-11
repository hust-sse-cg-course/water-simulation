#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <iostream>
#include <functional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"

class Texture {
public:
    enum WrapType {
        Repeat,
        Mirrored_Repeat,
        Clamp
    };

    Texture(int width, int height);
    Texture(std::string filepath, WrapType wraptype = WrapType::Repeat, GLenum type = GL_UNSIGNED_BYTE);

    void drawTo(std::function<void()> callback);
    void bind(unsigned int idx = 0);

    inline GLuint getTextureID() {
        return mTex;
    }

    inline GLenum getType() {
        return mType;
    }

    inline std::string getPath() {
        return mPath;
    }

    GLuint mTex;
    GLuint mFBO = 0, mRBO = 0;
    int mWidth, mHeight;
	GLenum mType;
    std::string mPath;
};

#endif // !__TEXTURE_H__
