# Memory in SDL3

![Screenshot of the game](in_game.png)

This is a simple Memory game with creepy themes.

It is developed with Simple DirectMedia Layer (SDL3).

All used assets are made by me.

## Playing the game

Windows, Linux and Web Builds are automatically uploaded and can be found on the Actions page.

The web build can be played on the [GitHub Page](https://f1sch.github.io/MemorySDL/).

## Building the game

The game can be built by yourself using the provided CMake presets.

* Clone the repo and build for the desired Platform:

#### Windows
* ```cmake --preset windows-release```
* ```cmake --build --preset windows-release```

The executable can be found in /MemorySDL/build/win/MemorySDL.exe

#### Linux
* ```cmake --preset linux-release```
* ```cmake --build --preset linux-release```

The executable can be found in /MemorySDL/build/linux/MemorySDL

#### Web
* ```cmake --preset web-release```
* ```cmake --build --preset web-release```

For the web build make sure [Emscripten](https://emscripten.org/docs/tools_reference/emsdk.html) is installed.

Run the /build/web/index.html with:

```emrun /build/web/index.html```

Make sure environment variables are set.

More info on the [Emscripten Documentation](https://emscripten.org/docs/compiling/index.html).

### License
The source code is open source, but all game assets (art, audio, music) are proprietary and may not be reused.
