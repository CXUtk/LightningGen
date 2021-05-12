#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Renderer.h"

struct LightningNode {
    glm::vec2 position;
    float branch;
    float energyDensity;
};

class LightningTree {
public:
    LightningTree(const std::vector<LightningNode>& keyNodes, glm::vec2 randomSeed, float alpha, float beta, float gamma);
    ~LightningTree();

    void Draw(const std::shared_ptr<Renderer>& renderer);
    void RunOneStep(const std::shared_ptr<Renderer>& renderer);
    void RunMultipleSteps(const std::shared_ptr<Renderer>& renderer, int steps);

private:
    glm::vec2 _seed;
    std::vector<LightningNode> _nodes;
    float _alpha, _beta, _gamma;

    GLuint _lightningVAO[2], _lightningVBO[2], _lightningTF[2];
    int _swapIndex;
    int _round;
    bool _firstTime;
};
