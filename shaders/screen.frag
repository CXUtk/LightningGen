#version 330 core
/*
    Input: [vec2] texture coords from vertex shader
    Output: [vec4] color of text render by textColor
*/

in vec2 vTexCoord;

out vec4 color;

uniform sampler2D uTexture;
uniform vec3 uColor;


float decode(vec4 c){
    return dot(c, 
        vec4(256.0, 1.0, 1.0 / 256.0, 1.0 / 65536.0));
}

vec4 encode(float v){
    vec4 bitshift = vec4(1.0 / 256.0, 1.0, 256.0, 65536.0);
    vec4 tmp = fract(v * bitshift);
    return vec4( tmp.xyz - tmp.yzw / 256.0, tmp.w ) + 1.0 / 512.0;
}


//    float strength = 0.0;
////    for(int i = 0; i < NUM_SAMPLES; i++) {
////        vec2 offset = poissonDisk[i] * 0.05;
////        vec4 c = texture(uScreenTexture, vTexCoord + offset);
////        strength += dot(c, vec4(256.0, 1.0, 1.0 / 256.0, 1.0 / 256.0 / 256.0)) * gauss(offset, 0.5);
////    }
//    for(int i = -10; i <= 10; i++){
//        for(int j = -10; j <= 10; j++){
//            vec2 offset = vec2(i, j) * 0.003;
//            vec4 c = texture(uScreenTexture, vTexCoord + offset);
//            strength += dot(c, vec4(256.0, 1.0, 1.0 / 256.0, 1.0 / 256.0 / 256.0)) * gauss(offset * 10, 0.1);
//        }
//    }
//    strength /= NUM_SAMPLES;

//    vec3 rColor = uColor * original;
//    rColor.x = pow(rColor.x, 1 / 2.2);
//    rColor.y = pow(rColor.y, 1 / 2.2);
//    rColor.z = pow(rColor.z, 1 / 2.2);

void main() {
    vec3 rColor = uColor * decode(texture(uTexture, vTexCoord));
    rColor.x = pow(rColor.x, 1 / 2.2);
    rColor.y = pow(rColor.y, 1 / 2.2);
    rColor.z = pow(rColor.z, 1 / 2.2);
    color = vec4(rColor, 1.0);
}