#version 130
// Wave parameters are all defined as inputs
//  for the vertex shader.
uniform float time, freq, power;
// uniform vec2 aspect;
varying float wave;
varying vec3 n, spacePos;

const float pi = 3.141592;

float WaveFun(float x, float t, float f, float p) {
    return pow((1.0 + sin(f * t * pi + x)) * 0.5, p);
}

void main() {
    gl_FrontColor = gl_Color;
    gl_TexCoord[0] = gl_MultiTexCoord0;
    vec4 vert = gl_Vertex;
    vec2 coord = vert.xz; // * vec2(aspect);
	wave = WaveFun(sqrt(dot(coord, coord)), time, freq, power);
    vert.y *= wave;
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vert;
    spacePos = gl_Position.xyz;
    n = gl_NormalMatrix * (gl_Normal * vec3(1.0, wave, 1.0));
}
