#version 450 core

layout (lines) in;
layout (points, max_vertices = 6) out;


in vec2 vPos[];
in float vBranch[];
in float vStrength[];

out vec2 pos;
out float branch;
out float strength;

uniform int Round;
uniform vec2 uRandomSeed;
uniform float uAlpha;

float randFloat(vec2 seed){
    return fract(sin(dot(seed + uRandomSeed, 
        vec2(1.61803398, 2.71348596)) * 104778.0));
}

float randFloat2(vec2 seed){
    return sin(dot(seed + uRandomSeed, 
        vec2(1.41421356, 2.6457513)) * 65535.0);
}

void main() {  
    // Start point
    pos =  vPos[0];
    branch = vBranch[0];
    strength = vStrength[0];
    EmitVertex();


    // Middle point
    vec2 coord = (vPos[0] + vPos[1]) * 0.5;
    float extend = randFloat2(coord);

    float len = length(vPos[1] - vPos[0]);
    vec2 unit = normalize(vPos[1] - vPos[0]);
    unit = vec2(-unit.y, unit.x);

    pos = (vPos[0] + vPos[1]) * 0.5 + uAlpha * len * extend * unit;
    branch = vBranch[0];
    strength = (vStrength[0] + vStrength[1]) * 0.5;
    EmitVertex();
    EmitVertex();

    vec2 tPos = pos;
    float tStrength = strength;
    float tBranch = branch;

    // End point
    pos =  vPos[1];
    branch = vBranch[0];
    strength = vStrength[1];
    EmitVertex();


    // Split point
    float prob = randFloat(coord);
    if ( Round - vBranch[0] < 5 && prob < 0.6 * exp(-0.4 * Round) ) {
        pos = tPos;
        strength = min(tStrength * 0.9, 2.0);
        branch = vBranch[0] + 1;
        EmitVertex();

        vec2 branchPos = tPos - randFloat(tPos) * unit;
        vec2 branchDir = normalize(normalize(tPos - vPos[0]) + extend * unit * randFloat(tPos));

        float branchLength = (tPos.y * 0.5 + 100) * exp(-0.5 * vBranch[0]) * (0.3 + 0.8 * randFloat(branchPos));
        pos = tPos + branchDir * branchLength;
        branch = vBranch[0] + 1;
        strength = 0;
        EmitVertex();
    }

    
    EndPrimitive();
}