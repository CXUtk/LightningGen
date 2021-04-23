#version 330 core

layout (location = 0) in vec4 vertex;

uniform mat4 transform;

out float branch;
out float strength;

void main(){
    gl_Position = transform * vec4(vertex.xy, 0, 1);
    branch = vertex.z;
    strength = vertex.w;
}

