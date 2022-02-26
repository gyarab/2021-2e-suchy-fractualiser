#version 400 core
out vec4 FragColor;
uniform ivec2 dimensions;
uniform int iterations;
uniform double offsetx;
uniform double offsety;
uniform double zoom;

void main() {
    double x = (gl_FragCoord.x-dimensions.x/2)/dimensions.y*zoom+offsetx;
    double y = (gl_FragCoord.y-dimensions.y/2)/dimensions.y*zoom+offsety;
    double ogx = x;
    double ogy = y;
    double tempx, tempy;
    bool suc = true;
    int i;
    for (i = 0; i < iterations; i++) {
         tempx = x*x - y*y + ogx;
         tempy = 2*y*x + ogy;
         x = tempx;
         y = tempy;
         if (x*x+y*y > 3000) { break; }
    }
    if (i == iterations) FragColor = vec4(0.7f, 0.0f, 0.0f, 1.0f);
    else FragColor = vec4(0.0f, 0.7f, 0.0f, 1.0f);
}
