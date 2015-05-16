# 4th SFML Game Jam

## Theme: From the end to the beginning

Not much to say yet; I'm still thinking about the theme, plus working on the
engine and boilerplate, so I don't know if I'll even get to writing an actual
game in time to submit it. :(

### Build Instructions

> **Note:** C++11 is required to build this project.  The default project
> settings will enable C++11 support for GCC 4.7+, but you may need to make
> adjustments to enable it in your toolchain.

```bash
git clone https://github.com/criptych/sfmljam4.git sfmljam4
cd sfmljam4
git submodule init
git submodule update
mkdir build
cd build
cmake ..
cmake --build .
```

If all goes well, you should have an executable file `untitled` (`untitled.exe`
on Windows) and a folder `data`.  (If the folder is missing, copy it from the
source directory.)

