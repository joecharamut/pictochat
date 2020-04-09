#version 130

uniform sampler2D sampler;

uniform vec2 size;

in vec4 color;
in vec2 texCoord;
out vec4 fragColor;

#define CURVATURE_X 0.2
#define CURVATURE_Y 0.3

//varying vec2 screenScale;

vec2 Distort(vec2 coord) {
    vec2 CURVATURE_DISTORTION = vec2(CURVATURE_X, CURVATURE_Y);
    // Barrel distortion shrinks the display area a bit, this will allow us to counteract that.
    vec2 barrelScale = 1.0 - (0.23 * CURVATURE_DISTORTION);
//    coord *= screenScale;
    coord -= vec2(0.5);
    float rsq = coord.x * coord.x + coord.y * coord.y;
    coord += coord * (CURVATURE_DISTORTION * rsq);
    coord *= barrelScale;
    if (abs(coord.x) >= 0.5 || abs(coord.y) >= 0.5) {
        coord = vec2(-1.0);// If out of bounds, return an invalid value.
    } else {
        coord += vec2(0.5);
//        coord /= screenScale;
    }

    return coord;
}

void main() {
    if (Distort(texCoord).x < 0.0) {
        fragColor = vec4(0.0);
    } else {
        fragColor = texture2D(sampler, Distort(texCoord)) * color;
    }
}

