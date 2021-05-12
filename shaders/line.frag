#version 330 core
out vec4 color;

in float branch;
in float strength;


float decode(vec4 c){
    return dot(c, 
        vec4(256.0, 1.0, 1.0 / 256.0, 1.0 / 65536.0));
}

vec4 encode(float v){
    vec4 bitshift = vec4(1.0 / 256.0, 1.0, 256.0, 65536.0);
    vec4 tmp = fract(abs(v) * bitshift);
    return vec4( tmp.xyz - tmp.yzw / 256.0, tmp.w ) + 1.0 / 512.0;
}

void main(){
    // pack the HDR data into RGBA
//    vec4 bitshift = vec4(1.0, 256.0, 256.0 * 256.0, 256.0 * 256.0 * 256.0);
//    color = mod(strength * bitshift, vec4(256.0, 256.0, 256.0, 256.0)) / 256.0;
    color = encode(strength);
}