a venture into the unknown

## Structure

```bash
$ tree
├───README.md
├───tools
│   └───genie{_linux, _mac, .exe}
├───scripts
│   └───genie.lua
├───.build
│   └───projects
├───3rdparty
│   └───SDL2 (windows)
|	└───vg-renderer
├───src
```

## Dependencies
 - [bgfx](https://github.com/bkaradzic/bgfx)
 - [bx](https://github.com/bkaradzic/bx)
 - [bimg](https://github.com/bkaradzic/bimg)
 - [SDL2](http://www.libsdl.org/download-2.0.php)
 - [vg-renderer](https://github.com/jdryg/vg-renderer)

## Building
 - `git clone https://github.com/smeets/autumn-jam`
 - `git submodule update --init`
 - install SDL2 (only linux/mac, windows is included)

### Windows
Use `Makefile.windows`: `project`, `build`, `start`, `shaders` and `clean`.

 - `cd autumn-jam`
 - Generate vs2017 sources: `nmake /f scripts\Makefile.windows project`
 - Run the program: `nmake /f scripts\Makefile.windows start`

### Linux/Mac
```bash
$ cd autumn-jam
# generate makefile in .build\projects\linux64_gmake (?)
$ tools/genie_linux gmake
```

## Shaders
Shaders should be compiled by a makefile, see the `Makefile.windows`.

```bash
export BIN_DIR = .build/path/to/exe/
export PLATFORM = windows || linux
mkdir -p $BIN_DIR/shaders/glsl
tools/shaderc -f shaders/fs_cube.sc -o $BIN_DIR/shaders/glsl/fs_cube.bin -i ../bgfx/src --type fragment --platform $PLATFORM
tools/shaderc -f shaders/vs_cube.sc -o $BIN_DIR/shaders/glsl/vs_cube.bin -i ../bgfx/src/ --type vertex --platform $PLATFORM
```