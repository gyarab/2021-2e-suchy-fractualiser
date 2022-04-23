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

