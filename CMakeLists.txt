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

project(RayOn_CPP_CMAKE C CXX)

set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_X11 ON CACHE BOOL "" FORCE)

set(CMAKE_CXX_STANDARD 20)

# Setting parameters for raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # or games

add_subdirectory(libs/raylib)

add_executable(${PROJECT_NAME}-raycast src/main-raycast.cpp
        src/RayOn/Materials/Material.h
        src/RayOn/SFDrawable.h
        src/RayOn/SFCircle.h
        src/RayOn/SFScene.cpp
        src/RayOn/SFScene.h
        src/RayOn/SFPoinInfo.h
        src/RayOn/SFSampler.cpp
        src/RayOn/SFSampler.h
        src/RayOn/SFBox.h
        src/System/ThreadSafeQueue.cpp
        src/System/ThreadSafeQueue.h
        src/json.hpp
        src/App/RayOnApp.cpp
        src/App/RayOnApp.h
        src/App/AppSettings.h
        src/Misc/StepInterval.h
        src/RayOn/Vec2.h
        src/RayOn/Vec3.h
        src/RayOn/SDFLUT.h
        src/System/ThreadPool.h
        src/System/Segment.h
        src/RayOn/SFTriangle.h
)
target_link_libraries(${PROJECT_NAME}-raycast PRIVATE raylib)
target_link_libraries(${PROJECT_NAME}-raycast PRIVATE glm::glm)
target_compile_definitions(${PROJECT_NAME}-raycast PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/") # Set the asset path macro to the absolute path on the dev machine
#target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="relative-path-to-assets-in-the-game-package") # Set the asset path macro in release mode

add_executable(${PROJECT_NAME}-rt src/main.cpp
        src/RayOn/Materials/Material.h
        src/RayOn/SFDrawable.h
        src/RayOn/SFCircle.h
        src/RayOn/SFScene.cpp
        src/RayOn/SFScene.h
        src/RayOn/SFPoinInfo.h
        src/RayOn/SFSampler.cpp
        src/RayOn/SFSampler.h
        src/RayOn/SFBox.h
        src/System/ThreadSafeQueue.cpp
        src/System/ThreadSafeQueue.h
        src/json.hpp
        src/App/RayOnApp.cpp
        src/App/RayOnApp.h
        src/App/AppSettings.h
        src/Misc/StepInterval.h
        src/RayOn/Vec2.h
        src/RayOn/Vec3.h
        src/RayOn/SDFLUT.h
        src/System/ThreadPool.h
        src/System/Segment.h
        src/RayOn/SFTriangle.h
)
target_link_libraries(${PROJECT_NAME}-rt PRIVATE raylib)
target_link_libraries(${PROJECT_NAME}-rt PRIVATE glm::glm)
target_compile_definitions(${PROJECT_NAME}-rt PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/") # Set the asset path macro to the absolute path on the dev machine
#target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="relative-path-to-assets-in-the-game-package") # Set the asset path macro in release mode

add_executable(${PROJECT_NAME}-plus-d-raytrace src/main-plus-d-raytrace.cpp
        src/json.hpp
        src/RayOn/RayOn.h
        src/App/RayOnApp.cpp
        src/App/RayOnApp.h
        src/App/AppSettings.h
        src/System/ThreadSafeQueue.cpp
        src/System/ThreadSafeQueue.h
        src/System/RTThreadPool.h
        src/System/Segment.h
        src/Misc/StepInterval.h
        src/Misc/Interval.h
        src/Misc/DiscreteInterval.h
        src/RayOn/Vec2.h
        src/RayOn/Vec3.h
        src/RayOn/Vec3.cpp
        src/RayOn/Ray3.h
        src/RayOn/Camera.h
        src/RayOn/plus-d/RTHitInfo.h
        src/RayOn/plus-d/RTDrawable.h
        src/RayOn/plus-d/Primitives/RTSphere.h
        src/RayOn/plus-d/Containers/RTScene.h
        src/RayOn/plus-d/Samplers/RTSampler.h
        src/RayOn/plus-d/Samplers/RTSampler.cpp
        src/RayOn/plus-d/Primitives/RTBox.h
        src/RayOn/plus-d/AABB.h
        src/RayOn/plus-d/BVH.h
        src/RayOn/plus-d/BVH.cpp
        src/RayOn/Textures/Texture.h
        src/RayOn/Textures/SolidColorTexture.h
        src/RayOn/Textures/CheckeredTexture.h
        src/RayOn/Materials/Material.h
        src/RayOn/Materials/SolidColorMaterial.h
        src/RayOn/Materials/SolidColorMaterial.cpp
        src/RayOn/Materials/ReflectingMaterial.h
        src/RayOn/Materials/ReflectingMaterial.cpp
        src/RayOn/RayOn.cpp
        src/RayOn/Materials/RefractingMaterial.h
        src/RayOn/Textures/LinesTexture.h
        src/RayOn/Materials/DiffusiveMaterial.h
        src/RayOn/Materials/DiffusiveMaterial.cpp
        src/RayOn/NoiseLUT.h
        src/RayOn/plus-d/Primitives/RTDisc.h
        src/RayOn/Textures/CheckeredColorTexture.h
        src/RayOn/Textures/ImageTexture.h
        src/Misc/Grid.h
        src/RayOn/plus-d/Primitives/RTQuad.h
        src/RayOn/plus-d/RTSceneCreator.h
        src/Scenes/MainScene.h
        src/Scenes/OpticalTest.h
        src/RayOn/plus-d/Primitives/RTEllipsoid.h
        src/Scenes/OpticalTest2.h
        src/RayOn/plus-d/Primitives/RTTriangle.h
        src/Scenes/TextureTest.h
        src/RayOn/Textures/ImageTexture.cpp
        src/Scenes/Tux.h
        src/RayOn/plus-d/Containers/RTMesh.h
        src/Scenes/ObjTest.h
        src/Scenes/SCHUM-BW.h
        src/RayOn/plus-d/Primitives/RTCylinder.h
        src/Scenes/Shapes.h
        src/Scenes/RayTraceTest.h
        src/RayOn/Materials/RefractingDispersed.h
        src/RayOn/plus-d/Primitives/RTHexPrism.h
        src/RayOn/Matrix4x4.h
        src/Scenes/RotateTranslate.h
        src/RayOn/Materials/DispersiveMaterial.h
        src/RayOn/Textures/ZigZagColorTexture.h
        src/Scenes/GlassScene.h
        src/Scenes/CORNEIL.h
        src/RayOn/plus-d/Transforms/RTRotate.h
        src/RayOn/plus-d/Transforms/RTTranslate.h
        src/RayOn/CMS/ONB.h
        src/Scenes/CORNEIL-BENCHY.h
        src/RayOn/CMS/PDF.h
        src/RayOn/Materials/IsotropicMaterial.cpp
        src/RayOn/Materials/IsotropicMaterial.h
        src/RayOn/plus-d/Transforms/RTFlipFace.h
        src/RayOn/plus-d/Containers/RTLightsList.h
        src/Scenes/SCHUM-BW-REFLECT.h
        src/RayOn/Color.cpp
        src/RayOn/Color.h
        src/RayOn/plus-d/Transforms/RTScale.h
        src/RayOn/plus-d/Primitives/RTPolygon.cpp
        src/RayOn/plus-d/Primitives/RTPolygon.h
        src/RayOn/CMS/CosLUT.h
        src/RayOn/CMS/SinLUT.h
        src/RayOn/plus-d/Primitives/RTLine.h
        src/RayOn/plus-d/Primitives/RTBBWireframe.h
        src/RayOn/Materials/DebugMaterials.cpp
        src/RayOn/Materials/DebugMaterials.h
        src/RayOn/Textures/WireframeTexture.h
        src/Scenes/CORNEIL-SKULL.h
        src/RayOn/Materials/MTLLoader.cpp
        src/RayOn/Materials/MTLLoader.h
        src/RayOn/Materials/RasterColorMaterial.cpp
        src/RayOn/Materials/RasterColorMaterial.h
)
target_link_libraries(${PROJECT_NAME}-plus-d-raytrace PRIVATE raylib)
target_link_libraries(${PROJECT_NAME}-plus-d-raytrace PRIVATE glm::glm)
target_compile_definitions(${PROJECT_NAME}-plus-d-raytrace PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/") # Set the asset path macro to the absolute path on the dev machine

add_executable(${PROJECT_NAME}-plus-d-raytrace-segmented src/main-plus-d-raytrace-segmented.cpp
        src/json.hpp
        src/RayOn/RayOn.h
        src/App/RayOnApp.cpp
        src/App/RayOnApp.h
        src/App/AppSettings.h
        src/System/ThreadSafeQueue.cpp
        src/System/ThreadSafeQueue.h
        src/System/RTThreadPool.h
        src/System/Segment.h
        src/Misc/StepInterval.h
        src/Misc/Interval.h
        src/Misc/DiscreteInterval.h
        src/RayOn/Vec2.h
        src/RayOn/Vec3.cpp
        src/RayOn/Vec3.h
        src/RayOn/Ray3.h
        src/RayOn/Camera.h
        src/RayOn/plus-d/RTHitInfo.h
        src/RayOn/plus-d/RTDrawable.h
        src/RayOn/plus-d/Primitives/RTSphere.h
        src/RayOn/plus-d/Containers/RTScene.h
        src/RayOn/plus-d/Samplers/RTSampler.h
        src/RayOn/plus-d/Samplers/RTSampler.cpp
        src/RayOn/plus-d/Primitives/RTBox.h
        src/RayOn/plus-d/AABB.h
        src/RayOn/plus-d/BVH.h
        src/RayOn/plus-d/BVH.cpp
        src/RayOn/Textures/Texture.h
        src/RayOn/Textures/SolidColorTexture.h
        src/RayOn/Textures/CheckeredTexture.h
        src/RayOn/Materials/Material.h
        src/RayOn/Materials/SolidColorMaterial.h
        src/RayOn/Materials/SolidColorMaterial.cpp
        src/RayOn/Materials/ReflectingMaterial.h
        src/RayOn/Materials/ReflectingMaterial.cpp
        src/RayOn/RayOn.cpp
        src/RayOn/Materials/RefractingMaterial.h
        src/RayOn/Textures/LinesTexture.h
        src/RayOn/Materials/DiffusiveMaterial.h
        src/RayOn/Materials/DiffusiveMaterial.cpp
        src/RayOn/NoiseLUT.h
        src/RayOn/plus-d/Primitives/RTDisc.h
        src/RayOn/Textures/CheckeredColorTexture.h
        src/RayOn/Textures/ImageTexture.h
        src/Misc/Grid.h
        src/RayOn/plus-d/Primitives/RTQuad.h
        src/RayOn/plus-d/RTSceneCreator.h
        src/Scenes/MainScene.h
        src/Scenes/OpticalTest.h
        src/RayOn/plus-d/Primitives/RTEllipsoid.h
        src/Scenes/OpticalTest2.h
        src/RayOn/plus-d/Primitives/RTTriangle.h
        src/Scenes/TextureTest.h
        src/RayOn/Textures/ImageTexture.cpp
        src/Scenes/Tux.h
        src/RayOn/plus-d/Containers/RTMesh.h
        src/Scenes/ObjTest.h
        src/Scenes/SCHUM-BW.h
        src/RayOn/plus-d/Primitives/RTCylinder.h
        src/Scenes/Shapes.h
        src/Scenes/RayTraceTest.h
        src/RayOn/Materials/RefractingDispersed.h
        src/RayOn/plus-d/Primitives/RTHexPrism.h
        src/RayOn/Matrix4x4.h
        src/Scenes/RotateTranslate.h
        src/RayOn/Materials/DispersiveMaterial.h
        src/RayOn/Textures/ZigZagColorTexture.h
        src/Scenes/GlassScene.h
        src/Scenes/CORNEIL.h
        src/RayOn/plus-d/Transforms/RTRotate.h
        src/RayOn/plus-d/Transforms/RTTranslate.h
        src/RayOn/CMS/ONB.h
        src/Scenes/CORNEIL-BENCHY.h
        src/RayOn/CMS/PDF.h
        src/RayOn/Materials/IsotropicMaterial.cpp
        src/RayOn/Materials/IsotropicMaterial.h
        src/RayOn/plus-d/Transforms/RTFlipFace.h
        src/RayOn/plus-d/Containers/RTLightsList.h
        src/Scenes/SCHUM-BW-REFLECT.h
        src/RayOn/Color.cpp
        src/RayOn/Color.h
        src/RayOn/plus-d/Transforms/RTScale.h
        src/RayOn/plus-d/Primitives/RTPolygon.cpp
        src/RayOn/plus-d/Primitives/RTPolygon.h
        src/RayOn/CMS/CosLUT.h
        src/RayOn/CMS/SinLUT.h
        src/RayOn/plus-d/Primitives/RTLine.h
        src/RayOn/plus-d/Primitives/RTBBWireframe.h
        src/RayOn/Materials/DebugMaterials.cpp
        src/RayOn/Materials/DebugMaterials.h
        src/RayOn/Textures/WireframeTexture.h
        src/Scenes/CORNEIL-SKULL.h
        src/RayOn/Materials/MTLLoader.cpp
        src/RayOn/Materials/MTLLoader.h
        src/RayOn/Materials/RasterColorMaterial.cpp
        src/RayOn/Materials/RasterColorMaterial.h
)
target_link_libraries(${PROJECT_NAME}-plus-d-raytrace-segmented PRIVATE raylib)
target_link_libraries(${PROJECT_NAME}-plus-d-raytrace-segmented PRIVATE glm::glm)
target_compile_definitions(${PROJECT_NAME}-plus-d-raytrace-segmented PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/") # Set the asset path macro to the absolute path on the dev machine


add_executable(${PROJECT_NAME}-plus-d-raytrace-realtime-a-bit src/main-plus-d-raytrace-realtime-a-bit.cpp
        src/json.hpp
        src/RayOn/RayOn.h
        src/App/RayOnApp.cpp
        src/App/RayOnApp.h
        src/App/AppSettings.h
        src/System/ThreadSafeQueue.cpp
        src/System/ThreadSafeQueue.h
        src/System/RTThreadPool.h
        src/System/Segment.h
        src/Misc/StepInterval.h
        src/Misc/Interval.h
        src/Misc/DiscreteInterval.h
        src/RayOn/Vec2.h
        src/RayOn/Vec3.h
        src/RayOn/Vec3.cpp
        src/RayOn/Ray3.h
        src/RayOn/Camera.h
        src/RayOn/plus-d/RTHitInfo.h
        src/RayOn/plus-d/RTDrawable.h
        src/RayOn/plus-d/Primitives/RTSphere.h
        src/RayOn/plus-d/Containers/RTScene.h
        src/RayOn/plus-d/Samplers/RTSampler.h
        src/RayOn/plus-d/Samplers/RTSampler.cpp
        src/RayOn/plus-d/Primitives/RTBox.h
        src/RayOn/plus-d/AABB.h
        src/RayOn/plus-d/BVH.h
        src/RayOn/plus-d/BVH.cpp
        src/RayOn/Textures/Texture.h
        src/RayOn/Textures/SolidColorTexture.h
        src/RayOn/Textures/CheckeredTexture.h
        src/RayOn/Materials/Material.h
        src/RayOn/Materials/SolidColorMaterial.h
        src/RayOn/Materials/SolidColorMaterial.cpp
        src/RayOn/Materials/ReflectingMaterial.h
        src/RayOn/Materials/ReflectingMaterial.cpp
        src/RayOn/RayOn.cpp
        src/RayOn/Materials/RefractingMaterial.h
        src/RayOn/Textures/LinesTexture.h
        src/RayOn/Materials/DiffusiveMaterial.h
        src/RayOn/Materials/DiffusiveMaterial.cpp
        src/RayOn/NoiseLUT.h
        src/RayOn/plus-d/Primitives/RTDisc.h
        src/RayOn/Textures/CheckeredColorTexture.h
        src/RayOn/Textures/ImageTexture.h
        src/Misc/Grid.h
        src/RayOn/plus-d/Primitives/RTQuad.h
        src/RayOn/plus-d/RTSceneCreator.h
        src/Scenes/MainScene.h
        src/Scenes/OpticalTest.h
        src/RayOn/plus-d/Primitives/RTEllipsoid.h
        src/Scenes/OpticalTest2.h
        src/RayOn/plus-d/Primitives/RTTriangle.h
        src/Scenes/TextureTest.h
        src/RayOn/Textures/ImageTexture.cpp
        src/Scenes/Tux.h
        src/RayOn/plus-d/Containers/RTMesh.h
        src/Scenes/ObjTest.h
        src/Scenes/SCHUM-BW.h
        src/RayOn/plus-d/Primitives/RTCylinder.h
        src/Scenes/Shapes.h
        src/Scenes/RayTraceTest.h
        src/RayOn/Materials/RefractingDispersed.h
        src/RayOn/plus-d/Primitives/RTHexPrism.h
        src/RayOn/Matrix4x4.h
        src/Scenes/RotateTranslate.h
        src/RayOn/Materials/DispersiveMaterial.h
        src/RayOn/Textures/ZigZagColorTexture.h
)
target_link_libraries(${PROJECT_NAME}-plus-d-raytrace-realtime-a-bit PRIVATE raylib)
target_link_libraries(${PROJECT_NAME}-plus-d-raytrace-realtime-a-bit PRIVATE glm::glm)
target_compile_definitions(${PROJECT_NAME}-plus-d-raytrace-realtime-a-bit PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/") # Set the asset path macro to the absolute path on the dev machine

# PDF test


add_executable(${PROJECT_NAME}-pdf-test src/main-pdf-test.cpp
       src/RayOn/RayOn.h
        src/App/RayOnApp.cpp
        src/App/RayOnApp.h
        src/App/AppSettings.h
        src/RayOn/Vec2.h
        src/RayOn/Vec3.h
        src/RayOn/Vec3.cpp
        src/RayOn/Ray3.h
        src/RayOn/RayOn.cpp
        src/RayOn/CMS/ONB.h
        src/RayOn/CMS/PDF.h
        src/main-pdf-test.cpp
)
target_link_libraries(${PROJECT_NAME}-pdf-test PRIVATE raylib)
target_link_libraries(${PROJECT_NAME}-pdf-test PRIVATE glm::glm)
target_compile_definitions(${PROJECT_NAME}-pdf-test PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/") # Set the asset path macro to the absolute path on the dev machine

add_executable(${PROJECT_NAME}-COS-LUT src/Tests/CosLUT.cpp
        src/RayOn/CMS/CosLUT.h
        src/RayOn/RayOn.h
        src/RayOn/RayOn.cpp
        src/Tests/CosLUT.cpp
)

set(SHADERS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src/shaders")

set(SHADERS_DEST_DIR "${CMAKE_BINARY_DIR}/shaders")

file(MAKE_DIRECTORY ${SHADERS_DEST_DIR})

set(SHADER_FILES
        light.glsl
        light_px.glsl
        debug.glsl
        sdf.glsl
        sdf_px.glsl
)

foreach(SHADER ${SHADER_FILES})
    set(SOURCE "${SHADERS_SOURCE_DIR}/${SHADER}")
    set(DEST "${SHADERS_DEST_DIR}/${SHADER}")

    configure_file(${SOURCE} ${DEST} COPYONLY)
endforeach()
