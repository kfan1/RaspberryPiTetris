Clone and see additional information on https://github.com/kfan1/RaspberryPiTetris.

## Running on Raspberry Pi
RaspberryPiTetris executable file already in build folder.

To compile and run on Raspberry Pi, make sure on master branch, run these two commands

```
cmake -S . -B build

cmake --build build
```

Executable file should be in build directory

## Running on Windows
To compile and run on Windows locally, make sure on windows-branch-local

Open solution in Visual Studio and add SDL and SDL_ttf as an existing project.

Add references to SDL3 and SDL_ttf. Build and run directly through Visual Studi.

Please follow these instructions if you need additional assistance: https://github.com/libsdl-org/SDL/blob/main/docs/INTRO-visualstudio.md
