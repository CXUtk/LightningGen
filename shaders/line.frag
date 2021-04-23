#version 330 core
out vec4 color;

in float branch;
in float strength;


void main(){
    // pack the HDR data into RGBA
    vec4 bitshift = vec4(1.0, 256.0, 256.0 * 256.0, 256.0 * 256.0 * 256.0);
    color = mod(abs(strength) * bitshift, vec4(256.0, 256.0, 256.0, 256.0)) / 256.0;
}