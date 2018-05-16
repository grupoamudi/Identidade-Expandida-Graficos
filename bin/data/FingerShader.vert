#version 150
// The vertex shader essentially takes the MVP matrix and
//  transforms the vertices and normals accordingly,
//  stretching or squasing them according to the "offset"
//  parameter.

uniform mat4 modelViewProjectionMatrix;
uniform float offset;
in vec4 position;
in vec3 normal;
out vec3 rawNormal, spacePos, screenPos;

void main() {
    vec4 transformPos = position * vec4(1.0, 1.0, offset, 1.0);
    spacePos = transformPos.xyz;
    transformPos = modelViewProjectionMatrix * transformPos;
    gl_Position = transformPos;
    screenPos = transformPos.xyz;
    vec4 intermediateNormal = vec4(normalize(normal * vec3(1.0, 1.0, 1.0 / offset)), 0.0) + transformPos;
    rawNormal = vec3(modelViewProjectionMatrix * intermediateNormal) - transformPos.xyz;
}
