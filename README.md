# Glint Renderer

This is my second attempt at an OpenGL-based renderer written in C++. I followed the Udemy course [Computer Graphics with Modern OpenGL and C++](https://www.udemy.com/course/graphics-with-modern-opengl/learn/) to get the basics down, including phong lighting, shadowmapping (directional and omnidirectional), models and textures.

I intend to continue working on it from time to time until I feel confident enough with graphics programming principles to move onto a newer API like Vulkan.

![Screenshot](content/screenshot.jpg)

## Project structure

* `assets`: Binary files ingested during runtime
* `external`: External libraries that aren't fetched by CMake
* `src/App`: Application-layer stuff
* `src/Renderer`: The bulk of the source - OpenGL abstractions, shader code, etc
* `src/Util`: Utility functions

## Setup

Requires Conan 2.0 or higher.

Run `conan install . --output-folder=build --build=missing --settings=build_type=Debug` to install dependencies.

Conan will generate `CMakeUserPresets.json`, which should inform CMake of the available presets for building the project.

## Stretch goals

- [x] Encapsulate renderer passes
- [ ] Improve shadow rendering
- [ ] Implement a post-processing pass with at least one effect (e.g. bloom)
- [ ] Improve the asset manager
- [ ] Improve asset loaders
- [ ] Implement particle system
- [ ] Implement debug UI
- [ ] Expose performance stats through debug UI
- [ ] Implement water rendering
