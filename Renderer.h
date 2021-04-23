#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>
#include "ShaderLoader.h"
#include "Shader.h"

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

    void DrawLines(GLuint VAO, float width, int points);
    void DrawSprite(GLuint VAO, float width, int points);

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

    glm::mat4 getCurrentTransform();
};