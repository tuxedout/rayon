cmake_policy(SET CMP0135 NEW)

cmake_minimum_required(VERSION 3.14) # FetchContent is new in version 3.11.

include(FetchContent)
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG origin/master
        DOWNLOAD_EXTRACT_TIMESTAMP true
)

FetchContent_GetProperties(glm)
if(NOT glm_POPULATED)
    FetchContent_Populate(glm)
    add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR})
endif()

#FetchContent_Declare(
#        googletest
#        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
#        DOWNLOAD_EXTRACT_TIMESTAMP true
#)
## For Windows: Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)

project(raylib_template C CXX)


set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_X11 ON CACHE BOOL "" FORCE)

set(CMAKE_CXX_STANDARD 20)

# Setting parameters for raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # or games

add_subdirectory(libs/raylib)

add_executable(${PROJECT_NAME}-raycast src/main-raycast.cpp
        src/RayOn/Material.h
        src/RayOn/SFDrawable.h
        src/RayOn/SFCircle.h
        src/RayOn/Scene.cpp
        src/RayOn/Scene.h
        src/RayOn/SFPoinInfo.h
        src/RayOn/SFSTracer.cpp
        src/RayOn/SFSTracer.h
        src/RayOn/SFBox.h
        src/System/ThreadSafeQueue.cpp
        src/System/ThreadSafeQueue.h
        src/json.hpp
        src/App/RayOnApp.cpp
        src/App/RayOnApp.h
        src/App/AppSettings.cpp
        src/App/AppSettings.h
        src/Misc/Interval.h
        src/RayOn/Vec2.h
        src/RayOn/Vec3.h
        src/RayOn/SDFLUT.cpp
        src/RayOn/SDFLUT.h
        src/System/ThreadPool.h
        src/System/Segment.h)
target_link_libraries(${PROJECT_NAME}-raycast PRIVATE raylib)
target_link_libraries(${PROJECT_NAME}-raycast PRIVATE glm::glm)
target_compile_definitions(${PROJECT_NAME}-raycast PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/") # Set the asset path macro to the absolute path on the dev machine
#target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="relative-path-to-assets-in-the-game-package") # Set the asset path macro in release mode

set(SHADERS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src/shaders")

set(SHADERS_DEST_DIR "${CMAKE_BINARY_DIR}/shaders")

file(MAKE_DIRECTORY ${SHADERS_DEST_DIR})

set(SHADER_FILES
        light.glsl
        debug.glsl
)

foreach(SHADER ${SHADER_FILES})
    set(SOURCE "${SHADERS_SOURCE_DIR}/${SHADER}")
    set(DEST "${SHADERS_DEST_DIR}/${SHADER}")

    configure_file(${SOURCE} ${DEST} COPYONLY)
endforeach()
