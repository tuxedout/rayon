cmake_minimum_required(VERSION 3.11) # FetchContent is new in version 3.11.

include(FetchContent)

FetchContent_Declare(
        glm
        GIT_REPOSITORY	https://github.com/g-truc/glm.git
        GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)

project(raylib_template C CXX)

set(CMAKE_CXX_STANDARD 17)

# Setting parameters for raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # or games

add_subdirectory(libs/raylib)

add_executable(${PROJECT_NAME} src/main.cpp
        src/RayOn/Material.cpp
        src/RayOn/Material.h
        src/RayOn/SFDrawable.cpp
        src/RayOn/SFDrawable.h
        src/RayOn/SFCircle.cpp
        src/RayOn/SFCircle.h
        src/RayOn/Scene.cpp
        src/RayOn/Scene.h
        src/RayOn/SFHitInfo.cpp
        src/RayOn/SFHitInfo.h
        src/RayOn/SFSTracer.cpp
        src/RayOn/SFSTracer.h
        src/RayOn/SFBox.cpp
        src/RayOn/SFBox.h
        src/System/ThreadSafeQueue.cpp
        src/System/ThreadSafeQueue.h)
add_executable(${PROJECT_NAME}-raycast src/main-raycast.cpp
        src/RayOn/Material.cpp
        src/RayOn/Material.h
        src/RayOn/SFDrawable.cpp
        src/RayOn/SFDrawable.h
        src/RayOn/SFCircle.cpp
        src/RayOn/SFCircle.h
        src/RayOn/Scene.cpp
        src/RayOn/Scene.h
        src/RayOn/SFHitInfo.cpp
        src/RayOn/SFHitInfo.h
        src/RayOn/SFSTracer.cpp
        src/RayOn/SFSTracer.h
        src/RayOn/SFBox.cpp
        src/RayOn/SFBox.h
        src/System/ThreadSafeQueue.cpp
        src/System/ThreadSafeQueue.h)
target_link_libraries(${PROJECT_NAME}-raycast PRIVATE raylib)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
target_link_libraries(${PROJECT_NAME}-raycast PRIVATE glm::glm)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/") # Set the asset path macro to the absolute path on the dev machine
#target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="relative-path-to-assets-in-the-game-package") # Set the asset path macro in release mode

# Установка переменной с путем к директории шейдеров
set(SHADERS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src/shaders")

# Установка переменной с целевой директорией для шейдеров
set(SHADERS_DEST_DIR "${CMAKE_BINARY_DIR}/shaders")

# Создание целевой директории для шейдеров
file(MAKE_DIRECTORY ${SHADERS_DEST_DIR})

# Список файлов шейдеров для копирования
set(SHADER_FILES
        light.glsl
        debug.glsl
)

# Цикл по всем файлам шейдеров для их копирования и обновления при изменении
foreach(SHADER ${SHADER_FILES})
    set(SOURCE "${SHADERS_SOURCE_DIR}/${SHADER}")
    set(DEST "${SHADERS_DEST_DIR}/${SHADER}")

    configure_file(${SOURCE} ${DEST} COPYONLY)
endforeach()