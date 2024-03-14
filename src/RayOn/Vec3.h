//
// Created by root on 14.03.2024.
//

#ifndef RAYON_CPP_CMAKE_VEC3_H
#define RAYON_CPP_CMAKE_VEC3_H

#include "Vec2.h"

namespace RN {
    struct Vec3 {
        int64_t x, y, z;

        Vec3(float _x, float _y, float _z) :
            x(static_cast<int64_t>(_x * SCALE_FACTOR)),
            y(static_cast<int64_t>(_y * SCALE_FACTOR)),
            z(static_cast<int64_t>(_z * SCALE_FACTOR)) {}

        Vec3(int64_t _x, int64_t _y, int64_t _z) : x(_x), y(_y), z(_z) {}

        [[nodiscard]] float getX() const { return static_cast<float>(x) / SCALE_FACTOR; }
        [[nodiscard]] float getY() const { return static_cast<float>(y) / SCALE_FACTOR; }
        [[nodiscard]] float getZ() const { return static_cast<float>(z) / SCALE_FACTOR; }

        Vec3 operator+(const Vec3& other) const {
            return {(x + other.x), (y + other.y), (z + other.z)};
        }

        Vec3 operator-(const Vec3& other) const {
            return {(x - other.x), (y - other.y) , (z - other.z)};
        }

        Vec3 operator*(const Vec3& other) const {
            return {(x * other.x), (y * other.y) , (z * other.z)};
        }

        Vec3 operator/(const int64_t &divider) const {
            return {x / static_cast<int64_t>(divider * SCALE_FACTOR), y /  static_cast<int64_t>(divider * SCALE_FACTOR), z /  static_cast<int64_t>(divider * SCALE_FACTOR)};
        }

        Vec3 operator/(const Vec3 &other) const {
            return {x / other.x, y / other.y, z / other.z};
        }

        Vec3 operator*(const int64_t &multiplier) const {
            return {(x * multiplier * SCALE_FACTOR), (y * multiplier * SCALE_FACTOR), (z * multiplier * SCALE_FACTOR)};
        }

        Vec3 operator*(const double &multiplier) const {
            return {(x * static_cast<int64_t>(multiplier * SCALE_FACTOR)), (y * static_cast<int64_t>(multiplier * SCALE_FACTOR)), (z * static_cast<int64_t>(multiplier * SCALE_FACTOR))};
        }

        [[nodiscard]] int64_t dotFix(const Vec3 &a) const{
            return a.x*x + a.y*y + a.z*a.z;
        }

        static float dot(const Vec3 &a, const Vec3 &b) {
            return static_cast<float>(a.dotFix(b));
        }

        void add(const Vec3 &a) {
            x += a.x;
            y += a.y;
            z += a.z;
        }

        void sub(const Vec3 &a) {
            x -= a.x;
            y -= a.y;
            z -= a.z;
        }

        void multiply(const Vec3 &a) {
            x *= a.x;
            y *= a.y;
            z *= a.z;
        }

        void

        [[nodiscard]] float dot(const Vec3 &a) const {
            return static_cast<float>(dotFix(a));
        }

        [[nodiscard]] float length() const {
            auto lengthSquared = static_cast<float>(x * x + y * y + z * z);

            return std::sqrt(lengthSquared) / SCALE_FACTOR;
        }

        [[nodiscard]] static float distance(const Vec3& v1, const Vec3& v2) {
            int64_t dx = v1.x - v2.x;
            int64_t dy = v1.y - v2.y;
            int64_t dz = v1.z - v2.z;

            int64_t distSquared = dx * dx + dy * dy + dz * dz;

            return std::sqrt(static_cast<float>(distSquared)) / SCALE_FACTOR;
        }
    };
}

#endif //RAYON_CPP_CMAKE_VEC3_H
