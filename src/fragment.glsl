#version 400 core
out vec4 FragColor;
uniform ivec2 dimensions;
uniform int iterations;
uniform double offsetx;
uniform double offsety;
uniform double zoom;
uniform sampler1D texture1;

void main() {
    double x = (gl_FragCoord.x-dimensions.x/2)/dimensions.y*zoom+offsetx;
    double y = (gl_FragCoord.y-dimensions.y/2)/dimensions.y*zoom+offsety;
    double ogx = x;
    double ogy = y;
    double tempx, tempy;
    bool suc = true;
    int i;
    for (i = 0; i < iterations; i++) {
         // tempx = x*x - y*y - 0.8;
         // tempy = 2*y*x + 0.156;
         tempx = x*x - y*y + ogx;
         tempy = 2*y*x + ogy;
         x = tempx;
         y = tempy;
         if (x*x+y*y > 3000000) { break; }
    }
    if (i == iterations) FragColor = texture(texture1, 0.0f);
    else FragColor = texture(texture1, float(i)/float(iterations)-0.5f);
}
