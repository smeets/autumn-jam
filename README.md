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
│   └───SDL2-2.0.6 (windows)
├───src
```

## Dependencies
 - [bgfx](https://github.com/bkaradzic/bgfx)
 - [bx](https://github.com/bkaradzic/bx)
 - [bimg](https://github.com/bkaradzic/bimg)
 - [SDL2](http://www.libsdl.org/download-2.0.php)

## Building
 - `git clone` dependencies into same parent folder as this repo. or use env. vars `{BGFX,BX,BIMG}_DIR` to specify the paths
 - install SDL2 (prebuilt windows lib is included)
 - generate project files: `cd autumn-jam && ./tools/genie <build-system>`
  - this will create the `.build/projects` folder
 - now build using the generated project (e.g. `make`)

```bash
# makefile
$ tools/genie_linux gmake

# visual studio
$ tools\\genie.exe vs2017
```