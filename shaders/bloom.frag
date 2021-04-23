#version 330 core
/*
    Input: [vec2] texture coords from vertex shader
    Output: [vec4] color of text render by textColor
*/

in vec2 vTexCoord;

out vec4 color;

uniform sampler2D uScreenTexture;
uniform vec3 uColor;

#define NUM_SAMPLES 20
#define PI 3.141592653589793
#define PI2 6.283185307179586

vec2 poissonDisk[NUM_SAMPLES];

highp float rand_1to1(highp float x ) { 
    return fract(sin(x) * 10000.0);
}

highp float rand_2to1(vec2 uv ) { 
	const highp float a = 12.9898, b = 78.233, c = 43758.5453;
	highp float dt = dot( uv.xy, vec2( a,b ) ), sn = mod( dt, PI );
	return fract(sin(sn) * c);
}

void uniformDiskSamples( const in vec2 randomSeed ) {

    float randNum = rand_2to1(randomSeed);
    float sampleX = rand_1to1( randNum ) ;
    float sampleY = rand_1to1( sampleX ) ;

    float angle = sampleX * PI2;
    float radius = sqrt(sampleY);

    for( int i = 0; i < NUM_SAMPLES; i ++ ) {
        poissonDisk[i] = vec2( radius * cos(angle) , radius * sin(angle)  );

        sampleX = rand_1to1( sampleY ) ;
        sampleY = rand_1to1( sampleX ) ;

        angle = sampleX * PI2;
        radius = sqrt(sampleY);
    }
}

float sqr(float x){
    return x * x;
}

float gauss(vec2 offset, float sigma) {
    float top = exp(-0.5 * (sqr(offset.x / sigma) + sqr(offset.y / sigma)));
    float bot = 2.0 * sqr(sigma) * PI;
    return top / bot;
}

void main() {
    // color = texture(uScreenTexture, vTexCoord);
    uniformDiskSamples(vTexCoord * 114514);
    float strength = 0.0;
    for(int i = 0; i < NUM_SAMPLES; i++) {
        vec2 offset = poissonDisk[i] * 0.05;
        vec4 c = texture(uScreenTexture, vTexCoord + offset);
        strength += dot(c, vec4(256.0, 1.0, 1.0 / 256.0, 1.0 / 256.0 / 256.0)) * gauss(offset, 0.5);
    }
    strength /= NUM_SAMPLES;
    float original = dot(texture(uScreenTexture, vTexCoord), vec4(256.0, 1.0, 1.0 / 256.0, 1.0 / 256.0 / 256.0));
    color = vec4(uColor * max(original, strength), 1.0);
}