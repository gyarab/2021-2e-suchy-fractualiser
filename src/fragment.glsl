#version 400 core
out vec4 FragColor;
uniform int width;
uniform int iterations;
uniform double offsetx;
uniform double offsety;
uniform double zoom;

void main() {
    int center = width/2;
    double x = (gl_FragCoord.x-center)/width*zoom-0.8+offsetx;
    double y = (gl_FragCoord.y-center)/width*zoom+offsety;
    double ogx = x;
    double ogy = y;
    bool suc = true;
    int i;
    for (i = 0; i < iterations; i++) {
         double tempx = x*x - y*y + ogx;
         double tempy = 2*y*x + ogy;
         x = tempx;
         y = tempy;
         if (x*x+y*y > 3000) { suc = false; break; }
    }
    if (suc) FragColor = vec4(0.7f, 0.0f, 0.0f, 1.0f);
    else FragColor = vec4(0.0f, 0.7f, 0.0f, 1.0f);
}
