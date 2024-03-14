//
// Created by root on 14.03.2024.
//

#ifndef RAYON_CPP_CMAKE_VEC2_H
#define RAYON_CPP_CMAKE_VEC2_H

#include <cstdint>
#include <cmath>

namespace RN {
    const int64_t SCALE_FACTOR = 100000000;

    struct Vec2 {
        int64_t x, y;

        Vec2(float _x, float _y) : x(static_cast<int64_t>(_x * SCALE_FACTOR)),
                                   y(static_cast<int64_t>(_y * SCALE_FACTOR)) {}

        Vec2(int64_t _x, int64_t _y) : x(_x), y(_y) {}

        [[nodiscard]] float getX() const { return static_cast<float>(x) / SCALE_FACTOR; }

        [[nodiscard]] float getY() const { return static_cast<float>(y) / SCALE_FACTOR; }
    };
}

#endif //RAYON_CPP_CMAKE_VEC2_H
