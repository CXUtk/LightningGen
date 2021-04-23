#version 450 core
/**
* Default Shader
*/
layout (location = 0) in vec4 vertex;

out vec2 vPos;
out float vBranch;
out float vStrength;

void main(){
    vPos = vertex.xy;
    vBranch = vertex.z;
    vStrength = vertex.w;
}

