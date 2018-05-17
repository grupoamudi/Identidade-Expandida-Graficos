#version 150
// The vertex shader essentially takes the MVP matrix and
//  transforms the vertices and normals accordingly,
//  stretching or squasing them according to the "offset"
//  parameter.

uniform mat4 modelViewProjectionMatrix;
//uniform float offset;
uniform float time;
in vec4 position;
in vec3 normal;
out vec3 rawNormal, spacePos, screenPos;
out float offsetV;

const float pi = 3.141592;

float WaveFun(float x, float t, float f, float p) {
    return 1.0 + 3.0 * pow((1.0 + sin(t * pi - pi * x)) * 0.5, p);
}

void main() {
    vec2 dist = position.xy;
    offsetV = WaveFun(sqrt(dot(dist, dist) * 0.0000005), time, 1.0, 2.0);
    
    vec4 transformPos = position * vec4(1.0, 1.0, offsetV, 1.0);
    spacePos = transformPos.xyz;
    transformPos = modelViewProjectionMatrix * transformPos;
    gl_Position = transformPos;
    screenPos = transformPos.xyz;
    vec4 intermediateNormal = vec4(normalize(normal * vec3(1.0, 1.0, 1.0 / offsetV)), 0.0) + transformPos;
    rawNormal = vec3(modelViewProjectionMatrix * intermediateNormal) - transformPos.xyz;
}
