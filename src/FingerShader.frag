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
varying vec3 n, spacePos, lightPos;

int XorShiftRNG(ivec3 input) {
    input ^= input << 13;
    input ^= input >> 17;
    input ^= input << 5;
    return input;
}

void main() {
    vec3 normal = normalize(n);
    float lightDiff = clamp(dot(normal, normalize(lightPos - spacePos)), 0.0, 1.0);
    vec3 noise = frac(vec3(XorShiftRNG(ivec3(lightPos * 65536.0))) / 65536.0);
    gl_FragColor.rgb = texture1D(palette, wave).rgb * (ambient + diffuse * lightDiff + noisePower * noise * exp(spacePos.z));
    gl_FragColor.a = 1.0;
}
