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
    Texture(std::string filepath, std::string type);
    Texture(std::string filepath, std::string type, WrapType wraptype);

    void drawTo(std::function<void()> callback);
    void bind(unsigned int idx = 0);

    inline GLuint getTextureID() {
        return mTex;
    }

    inline std::string getName() {
        return mType;
    }

    inline std::string getPath() {
        return mPath;
    }

    GLuint mTex;
    GLuint mFBO = 0, mRBO = 0;
    int mWidth, mHeight;
    std::string mType;
    std::string mPath;
};

#endif // !__TEXTURE_H__
