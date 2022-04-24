#version 400 core
out vec4 FragColor;
uniform ivec2 dimensions;
uniform int iterations;
uniform double offsetx;
uniform double offsety;
uniform double zoom;
uniform sampler1D texture1;

#define mult(a, b) dvec2(a.x*b.x-a.y*b.y, a.y*b.x+a.x*b.y)
#define div(a, b) dvec2((a.x*b.x+a.y*b.y)/(b.x*b.x+b.y*b.y),(a.y*b.x+a.x*b.y)/(b.x*b.x+b.y*b.y))

void main() {
    double zr = (gl_FragCoord.x-dimensions.x/2)/dimensions.y*zoom+offsetx;
    double zi = (gl_FragCoord.y-dimensions.y/2)/dimensions.y*zoom+offsety;
    dvec2 z = dvec2(zr, zi);
    dvec2 c = z;
    int i;
    for (i = 0; i < iterations; i++) {
         %formula%
         if (z.x*z.x+z.y*z.y > 900) { break; }
    }
    if (i == iterations) FragColor = texture(texture1, 1.0f);
    else FragColor = texture(texture1, float(i)/float(iterations));
}
