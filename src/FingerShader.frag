#version 130
// The fragment shader takes a color pallete
//  accessed according to the "wave" parameter
//  output from the vertex shader.
//
// I'm still undecided if we leave the wave
//  function calculated per vertex (gouraud
//  shading), or move to calculate it per
//  fragment (possibly prettier, but depends
//  on power of final target GPU).

uniform sampler1D palette;
uniform float ambient, diffuse, noisePower;
varying float wave;
varying vec3 n, spacePos;

vec3 XorShiftRNG(vec3 seed) {
    ivec3 is = ivec3(seed * 65536.0);
    is ^= is << 13;
    is ^= is >> 17;
    is ^= is << 5;

    return vec3(frac(vec3(is) / 65536.0));
}

const float pi = 3.141592;

float WaveFun(float x, float t, float f, float p) {
    return pow((1.0 + sin(f * t * pi + x)) * 0.5, p);
}

void main() {
    vec3 normal = normalize(n);
    normal = normalize(n + noisePower * noise * exp(spacePos.z));
    float lightDiff = clamp(dot(normal, normalize(gl_LightPosition[0] - spacePos)), 0.0, 1.0);
    vec3 noise = XorShiftRNG(spacePos);
    gl_FragColor.rgb = texture1D(palette, wave).rgb * (ambient + diffuse * lightDiff);
    gl_FragColor.a = 1.0;
}
