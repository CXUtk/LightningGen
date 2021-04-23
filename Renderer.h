#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>
#include "ShaderLoader.h"
#include "Shader.h"

class Renderer {
public:
    Renderer(int width, int height);
    ~Renderer();

    void ClearCurrentFrame();
    void CreateNewFrameBuffer(int width, int height);
    void SwitchFrameBuffer();
    void ResetFrameBuffer();


    void Begin(glm::mat4 transformMatrix);
    void End();

    void DrawLines(GLuint VAO, float width, int points);
    void DrawLightningToScreen();

    std::shared_ptr<Shader> GetGeometryShader() const { return _geometryShader; }

private:
    GLuint _fbo;
    GLuint _fboTexture;

    struct DrawState {
        glm::mat4 transform;
    };

    static constexpr int MAXN = 64;
    DrawState stack[MAXN];
    int stacktop;

    std::shared_ptr<Shader> _geometryShader;
    std::shared_ptr<Shader> _lineShader;
    std::shared_ptr<Shader> _bloomShader;

    GLuint _mainVAO, _mainVBO;

    glm::mat4 getCurrentTransform();
    void initialize(int width, int height);
};