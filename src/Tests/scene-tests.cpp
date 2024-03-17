#include <gtest/gtest.h>

#include <gtest/gtest.h>
#include "../RayOn/Scene.h"

const int screen_width = 1400;
const int screen_height = 700;

RN::Scene scene(screen_width, screen_height);

TEST(Scene, CoordsConv) {
    RN::vec2d scene_coords(0,0);
    RN::vec2i px_coords = scene.pxCoords(scene_coords);
    ASSERT_NEAR(px_coords.x, 700, 1e-6);
    ASSERT_NEAR(px_coords.y, 350, 1e-6);

    scene_coords = {-1.0, 0.5};

    px_coords = scene.pxCoords(scene_coords);
    ASSERT_NEAR(px_coords.x,   0, 1e-6);
    ASSERT_NEAR(px_coords.y, 700, 1e-6);

    scene_coords = scene.sceneCoords(px_coords);
    ASSERT_NEAR(scene_coords.x, -1.0, 1e-6);
    ASSERT_NEAR(scene_coords.y,     0.5, 1e-6);

    scene_coords = {0.5, -0.3};

    px_coords = scene.pxCoords(scene_coords);
    ASSERT_NEAR(px_coords.x, 1050, 1e-6);
    ASSERT_NEAR(px_coords.y,     140, 1e-6);

    scene_coords = {-1.0, -0.5};

    px_coords = scene.pxCoords(scene_coords);
    ASSERT_NEAR(px_coords.x, 0, 1e-6);
    ASSERT_NEAR(px_coords.y, 0, 1e-6);
}
