#include "LightningTree.h"

static constexpr int BUFFER_SIZE = 1 << 20;
static constexpr int MAX_POINTS = 1 << 15;

LightningTree::LightningTree(const std::vector<LightningNode>& keyNodes, glm::vec2 randomSeed, float alpha, float beta)
: _nodes(keyNodes), _seed(randomSeed), _alpha(alpha), _beta(beta) {

    _swapIndex = 0;
    _round = 0;
    _firstTime = true;

    glGenVertexArrays(2, &_lightningVAO[0]);
    glGenBuffers(2, &_lightningVBO[0]);
    glGenTransformFeedbacks(2, &_lightningTF[0]);

    for (int i = 0; i < 2; i++) {
        glBindVertexArray(_lightningVAO[i]);

        glBindBuffer(GL_ARRAY_BUFFER, _lightningVBO[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(LightningNode) * BUFFER_SIZE, nullptr, GL_DYNAMIC_DRAW);
        
        glVertexAttribPointer(0, sizeof(LightningNode) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(LightningNode), (void*)0);
        glEnableVertexAttribArray(0);
    }

    // Let the two transform feedback buffers point to the two VBO buffers.
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, _lightningTF[0]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, _lightningVBO[0]);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, _lightningTF[1]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, _lightningVBO[1]);


    glBindVertexArray(_lightningVAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, _lightningVBO[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(LightningNode) * 2, keyNodes.data());
    glBindVertexArray(0);
}
  
LightningTree::~LightningTree() {
}

void LightningTree::Draw(const std::shared_ptr<Renderer>& renderer) {
    renderer->DrawLines(_lightningVAO[_swapIndex], 1, MAX_POINTS);
}

void LightningTree::RunOneStep(const std::shared_ptr<Renderer>& renderer) {

    auto shader = renderer->GetGeometryShader();
    shader->Apply();
    shader->SetParameter<int>("Round", _round);
    shader->SetParameter<glm::vec2>("uRandomSeed", _seed);
    shader->SetParameter<float>("uAlpha", _alpha);

    glBindVertexArray(_lightningVAO[_swapIndex]);
    // Transform feedback goes into the other VBO.
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, _lightningTF[1 - _swapIndex]);
    glBeginTransformFeedback(GL_POINTS);

    glDrawArrays(GL_LINES, 0, MAX_POINTS);

    glEndTransformFeedback();

    // Alternate between buffers
    _swapIndex = 1 - _swapIndex;      

    //GLint size = 0;
    //glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    printf("Gen %d\n", _round);
    ++_round;

    glBindVertexArray(0);
}

void LightningTree::RunMultipleSteps(const std::shared_ptr<Renderer>& renderer, int steps) {
}
