#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>

class Renderer {
public:
    Renderer();
    ~Renderer();

    void ClearCurrentFrame();
    void CreateNewFrameBuffer(int width, int height);
    void SwitchFrameBuffer();
    void ResetFrameBuffer();


    void Begin(glm::mat4 transformMatrix);
    void End();

private:
    GLuint _fbo;
    GLuint _fboTexture;

    struct DrawState {
        glm::mat4 transform;
    };

    static constexpr int MAXN = 64;
    DrawState stack[MAXN];
    int stacktop;


    glm::mat4 getCurrentTransform();
};