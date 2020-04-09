#version 130

uniform sampler2D sampler;

uniform vec2 size;

in vec4 color;
in vec2 texCoord;
out vec4 fragColor;


// Apply radial distortion to the given coordinate.
#define distortion 0.0
// Controls the intensity of the barrel distortion used to emulate the
// curvature of a CRT. 0.0 is perfectly flat, 1.0 is annoyingly
// distorted, higher values are increasingly ridiculous.
vec2 radialDistortion(vec2 coord) {
    vec2 cc = coord - 0.5;
    float dist = dot(cc, cc) * distortion;
    return (coord + cc * (1.0 + dist) * dist);
}

void main() {
    fragColor = texture2D(sampler, texCoord) * color;
}

