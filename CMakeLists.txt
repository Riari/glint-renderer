cmake_minimum_required(VERSION 3.28)
project(udemy-opengl LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    glew
    GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
    GIT_TAG glew-cmake-2.2.0
)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.9
)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.12.0
)

FetchContent_MakeAvailable(glew glm spdlog)

FetchContent_GetProperties(glfw)
if(NOT glfw_POPULATED)
    FetchContent_Populate(glfw)

    set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
    set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
    set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
    set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")

    add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif()

set(SOURCES
    src/App/Camera.cpp
    src/App/Window.cpp
    src/GL/Buffer/BufferObject.cpp
    src/GL/Buffer/EBO.cpp
    src/GL/Buffer/VAO.cpp
    src/GL/Buffer/VBO.cpp
    src/GL/Mesh.cpp
    src/GL/Object.cpp
    src/GL/Shader.cpp
    src/GL/ShaderProgram.cpp
    src/GL/Texture.cpp
    src/Util/File.cpp
    src/Util/GL.cpp
    src/main.cpp
)

set(HEADERS
    src/App/Camera.h
    src/App/Window.h
    src/GL/Buffer/BufferObject.h
    src/GL/Buffer/EBO.h
    src/GL/Buffer/VAO.h
    src/GL/Buffer/VBO.h
    src/GL/Mesh.h
    src/GL/Object.h
    src/GL/Shader.h
    src/GL/ShaderProgram.h
    src/GL/Texture.h
    src/Util/File.h
    src/Util/GL.h
    src/Util/Random.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

add_subdirectory(external)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} PRIVATE libglew_static glfw glm spdlog stb)

# Copy assets
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)

# Copy shaders
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders)
