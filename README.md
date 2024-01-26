# udemy-opengl

Following [Computer Graphics with Modern OpenGL and C++](https://www.udemy.com/course/graphics-with-modern-opengl/learn/).

Project structure:

* `assets`: Binary files ingested during runtime
* `external`: External libraries that aren't fetched by CMake
* `src/App`: Application-layer stuff
* `src/GL`: OpenGL integration stuff
* `src/shaders`: Shader code
* `src/Util`: Utility functions
* `src/World`: Stuff that exists in the "world"

## Setup

Requires Conan 2.0 or higher.

Run `conan install . --output-folder=build --build=missing --settings=build_type=Debug` to install dependencies.

Conan will generate `CMakeUserPresets.json`, which should inform CMake of the available presets for building the project.
