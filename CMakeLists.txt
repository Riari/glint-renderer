cmake_minimum_required(VERSION 3.27)
project(udemy-opengl LANGUAGES CXX)

find_package(glew)
find_package(glfw3)
find_package(glm)
find_package(nlohmann_json)
find_package(spdlog)

set(SOURCES
    src/App/Window.cpp
    src/Renderer/GL/Buffer/BufferObject.cpp
    src/Renderer/GL/Buffer/EBO.cpp
    src/Renderer/GL/Buffer/VAO.cpp
    src/Renderer/GL/Buffer/VBO.cpp
    src/Renderer/GL/Object.cpp
    src/Renderer/GL/Shader.cpp
    src/Renderer/GL/ShaderProgram.cpp
    src/Renderer/GL/Texture.cpp
    src/Renderer/Light/DirectionalLight.cpp
    src/Renderer/Light/Light.cpp
    src/Renderer/Light/PointLight.cpp
    src/Renderer/Light/SpotLight.cpp
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
    src/Asset/Type/Image.h
    src/Asset/Asset.h
    src/Asset/Manager.h
    src/Renderer/GL/Buffer/BufferObject.h
    src/Renderer/GL/Buffer/EBO.h
    src/Renderer/GL/Buffer/VAO.h
    src/Renderer/GL/Buffer/VBO.h
    src/Renderer/GL/Object.h
    src/Renderer/GL/Shader.h
    src/Renderer/GL/ShaderProgram.h
    src/Renderer/GL/Texture.h
    src/Renderer/Light/DirectionalLight.h
    src/Renderer/Light/Light.h
    src/Renderer/Light/PointLight.h
    src/Renderer/Light/SpotLight.h
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

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} PRIVATE
    GLEW::GLEW
    glfw
    glm::glm
    nlohmann_json::nlohmann_json
    spdlog::spdlog
    stb)

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
                ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/shaders
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders)
