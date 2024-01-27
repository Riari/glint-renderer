cmake_minimum_required(VERSION 3.28)
project(udemy-opengl LANGUAGES CXX)

find_package(glew)
find_package(glfw3)
find_package(glm)
find_package(spdlog)

set(SOURCES
    src/App/Window.cpp
    src/GL/Buffer/BufferObject.cpp
    src/GL/Buffer/EBO.cpp
    src/GL/Buffer/VAO.cpp
    src/GL/Buffer/VBO.cpp
    src/GL/Object.cpp
    src/GL/Shader.cpp
    src/GL/ShaderProgram.cpp
    src/GL/Texture.cpp
    src/Renderer/Light/DirectionalLight.cpp
    src/Renderer/Light/Light.cpp
    src/Renderer/Light/PointLight.cpp
    src/Renderer/Camera.cpp
    src/Renderer/Map.cpp
    src/Renderer/Material.cpp
    src/Renderer/Model.cpp
    src/Util/File.cpp
    src/Util/GL.cpp
    src/main.cpp
)

set(HEADERS
    src/App/Window.h
    src/GL/Buffer/BufferObject.h
    src/GL/Buffer/EBO.h
    src/GL/Buffer/VAO.h
    src/GL/Buffer/VBO.h
    src/GL/Object.h
    src/GL/Shader.h
    src/GL/ShaderProgram.h
    src/GL/Texture.h
    src/Renderer/Light/DirectionalLight.h
    src/Renderer/Light/Light.h
    src/Renderer/Light/PointLight.h
    src/Renderer/Camera.h
    src/Renderer/Map.h
    src/Renderer/Material.h
    src/Renderer/Model.h
    src/Util/File.h
    src/Util/GL.h
    src/Util/Random.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

add_subdirectory(external)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW glfw glm::glm spdlog::spdlog stb)

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
