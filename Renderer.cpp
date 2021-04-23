#include "Renderer.h"

Renderer::Renderer(int width, int height) {
    stacktop = 0;
    memset(stack, 0, sizeof(stack));

    const char* feedbackValues[3] = { "pos", "branch", "strength" };
    _geometryShader = std::make_shared<Shader>(loadVertexGeometryShader("lightning_gen.vs", "lightning_gen.geom", feedbackValues, 3));
    _lineShader = std::make_shared<Shader>(loadVertexFragmantShader("line.vs", "line.frag"));
    _bloomShader = std::make_shared<Shader>(loadVertexFragmantShader("bloom.vs", "bloom.frag"));

    initialize(width, height);
}

Renderer::~Renderer() {
}

void Renderer::ClearCurrentFrame() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::CreateNewFrameBuffer(int width, int height) {
    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
   

    glGenTextures(1, &_fboTexture);
    glBindTexture(GL_TEXTURE_2D, _fboTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _fboTexture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::SwitchFrameBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
}

void Renderer::ResetFrameBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::Begin(glm::mat4 transformMatrix) {
    stack[++stacktop] = DrawState{ transformMatrix };
}

void Renderer::End() {
    --stacktop;
}

void Renderer::DrawLines(GLuint VAO, float width, int points) {
    _lineShader->Apply();
    glLineWidth(width);

    _lineShader->SetParameter<glm::mat4>("transform", getCurrentTransform());

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, points);
    glBindVertexArray(0);
}

void Renderer::DrawLightningToScreen() {
    _bloomShader->Apply();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _fboTexture);

    _bloomShader->SetParameter<glm::mat4>("transform", getCurrentTransform());
    _bloomShader->SetParameter<glm::vec3>("uColor", glm::vec3(0.1, 0.2, 1.0));
    _bloomShader->SetParameter<int>("uScreenTexture", 0);


    glBindVertexArray(_mainVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}


glm::mat4 Renderer::getCurrentTransform() {
    return stack[stacktop].transform;
}

void Renderer::initialize(int width, int height) {
    static float vertices[] = {
        0.f, 0.f, 0.f, 0.f,
        width, 0.f, 1.f, 0.f,
        0.f, height, 0.f, 1.f,
        0.f, height, 0.f, 1.f,
        width, 0.f, 1.f, 0.f,
        width, height, 1.f, 1.f,
    };

    // buffer for drawing triangles
    glGenVertexArrays(1, &_mainVAO);
    glGenBuffers(1, &_mainVBO);

    glBindVertexArray(_mainVAO);
    glBindBuffer(GL_ARRAY_BUFFER, _mainVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * 6, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)sizeof(glm::vec2));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

