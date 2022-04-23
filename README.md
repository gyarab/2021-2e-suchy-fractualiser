<h2 align="center">Fractualiser</h2>

Fractualiser je program pro výkres různých fraktálů.

### Dependence
Fractualiser má dvě dependence: OpenGL 4 a GLFW 3. Pro hladký průběh kompilace
je také pro obě dependence potřeba CMake Toolchain, aby je bylo možné najít
pomocí `find_package`.

Třetí dependence: GLAD je vygenerovaná v repozitáři ve složce `thirdparty/`.

### Kompilace
Pro zkompilování na linuxu stačí pár příkazů:
```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```
Následně můžete program spustit pomocí: `./fractualiser -h`

### Použití
```
Usage: fractualiser [options] [formula]
  -h, --help       Shows this help message
  -m, --multiplier How many times should a big render be bigger (default: 4)
  formula          The formula to calculate (default: z*z+c)
                   Supported operations: +-*/ with complex numbers
                   c - position of the current pixel in the complex plane
                   z - the iterated and final computed value

Controls: Use your mouse or ASDF (E-zoom in, Q-zoom out) to control the camera.
  F - increase iteration count, C - decrase iteration count
  G - print debug information and camera position
  P - create a big render - the output is in image.bmp
  ESC - exit the program

Fractualiser (fract(al)(vis)ualiser) is a program to render fractals using
OpenGL on a GPU.
```
