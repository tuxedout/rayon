//
// Created by tux on 14.03.2024.
//

#ifndef RAYON_CPP_CMAKE_VEC3_H
#define RAYON_CPP_CMAKE_VEC3_H

#include "Vec2.h"

namespace RN {
    struct vec3 {
    public:
        double x, y, z;

        vec3(double _x, double _y, double _z) :
            x((_x)),
            y((_y)),
            z((_z)) {}

        vec3(const vec3 &other) = default;


        vec3(): x(0), y(0), z(0) {}

        [[nodiscard]] vec3 abs() const {
            return {std::abs(x), std::abs(y), std::abs(z)};
        }

        void normalize() {
            auto len = length();
            if (len == 0) return;
            x = ((x) / len);
            y = ((y) / len);
            z = ((z) / len);
        }

        vec3 operator+(const vec3 &other) const {
            return {(x + other.x), (y + other.y), (z + other.z)};
        }

        vec3 operator+=(const float &other) const {
            return {x + (other), y + (other), z + (other) };
        }

        vec3 operator+=(const vec3 &other) const {
            return {x + other.x, y + other.y, z + other.z };
        }

        vec3 operator+(const float &other) const {
            return {x + (other), y + (other), z + (other) };
        }

        vec3 operator-(const vec3 &other) const {
            return {(x - other.x), (y - other.y) , (z - other.z)};
        }

        vec3 operator-(const float &other) const {
            return {x - (other), y - (other), z - (other) };
        }

        vec3 operator/(const double &divider) const {
            return {x / divider, y / divider, z / divider};
        }

        vec3 operator*(const double &multiplier) const {
            return {(x * multiplier), (y * (multiplier)), (z * (multiplier))};
        }

        vec3 operator*(const vec3 &multiplier) const {
            return {(x * ((multiplier.x))), (y * ((multiplier.y))), (z * ((multiplier.z)))};
        }

        void add(const vec3 &a) {
            x += a.x;
            y += a.y;
            z += a.z;
        }

        void add(const double &a) {
            x += (a) ;
            y += (a);
            z += (a);
        }


        void sub(const vec3 &a) {
            x -= a.x;
            y -= a.y;
            z -= a.z;
        }

        void sub(const double &a) {
            x -= (a);
            y -= (a);
            z -= (a);
        }

        void multiply(const double &a) {
            x *= (a);
            y *= (a);
            z *= (a);
        }

        void div(const double &a) {
            x /= (a);
            y /= (a);
            z /= (a);
        }

        [[nodiscard]] static double dot(const vec3 &a, const vec3 &b){
            return a.x*b.x + a.y*b.y + a.z*b.z;
        }

        [[nodiscard]] double dot(const vec3 &a) const {
            return a.x*x + a.y*y + a.z*z;
        }

        [[nodiscard]] double length() const {
            auto lengthSquared = (x * x + y * y + z * z);

            return std::sqrt(lengthSquared);
        }

        [[nodiscard]] static double distance(const vec3& v1, const vec3& v2) {
            double dx = v1.x - v2.x;
            double dy = v1.y - v2.y;
            double dz = v1.z - v2.z;

            double distSquared = dx * dx + dy * dy + dz * dz;

            return std::sqrt(distSquared) ;
        }

        [[nodiscard]] double distance(const vec3& a) const {
            double dx = x - a.x;
            double dy = y - a.y;
            double dz = z - a.z;

            double distSquared = dx * dx + dy * dy + dz * dz;

            return std::sqrt(distSquared);
        }
    };
}

#endif //RAYON_CPP_CMAKE_VEC3_H
