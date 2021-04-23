#version 330 core
/**
* Default Shader
*/
layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 texCoord;

uniform mat4 transform;

out vec2 vTexCoord;

void main(){
    gl_Position = transform * vec4(vertex, 0.0, 1.0);
    vTexCoord = texCoord;
}

