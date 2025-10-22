/*
 *
 *       Created by tux on 14.03.2024.
 *       ________   _______  ____ ____  _______  ____ ____
 *      │----R---\ /---A---\ ----Y---- /---O---\│----N----\
 *      │         │         │    │    │         │         │
 *      │    ^    │    ^    │    │    │    ^    │    ^    │
 *      │    │    │    │    │    │    │    │    │    │    │
 *      │    │    │    │    │    │    │    │    │    │    │
 *      │    ┼    │    ┼    │    ┼    │    ┼    │    │    │
 *      │        (          \         │    │    │    │    │
 *      │    ^    │    ^    │)        │    │    │    │    │
 *      │    │    │    │    /         │    v    │    │    │
 *      │    │    │    │    │        /│         │    │    │
 *      │────│────│────│────│───────/  \_______/│____│____│
 *
 *      RayOn - simple rig to play with rays
 *
 *      vec3 represents 3D
 *
 */

#ifndef RAYON_CPP_CMAKE_VEC3_H
#define RAYON_CPP_CMAKE_VEC3_H

#include "Vec2.h"
#include "RayOn.h"
#include <chrono>
#include <cstdlib>
#include <random>
#include <iostream>

namespace RN {
    template<typename T>
    struct vec3 {
    private:
        // random generator stuff
        static std::mt19937_64 engine;
        static std::uniform_real_distribution<double> dist;
    public:
        T x, y, z;

        static const vec3<double> ZERO;
        static const vec3<double> ONES;
        static const vec3<double> MAX;

        static vec3<double> random_unit_vector();

        static vec3<double> random_cosine_direction();

        vec3(T _v) : x(_v), y(_v), z(_v) {}

        vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

        vec3(const vec3 &other) = default;

        vec3() : x(0), y(0), z(0) {}

        static vec3 random() {
            return vec3(RN::random(), RN::random(), RN::random());
        }

        static vec3 random(T min, T max) {
            T range = max - min;
            return vec3(min + RN::random() * range, min + RN::random() * range, min + RN::random() * range);
        }

        static vec3 random_in_unit_disk() {
            while (true) {
                auto p = vec3(RN::random(-1,1), RN::random(-1,1), 0);
                if (p.length_sqr() < 1)
                    return p;
            }
        }

        static vec3 random_on_hemisphere(const vec3& normal) {
            vec3 on_unit_sphere = random_unit_vector();
            if (dot(on_unit_sphere, normal) > 0.0) {
                return on_unit_sphere;
            } else {
                return -on_unit_sphere;
            }
        }

        [[nodiscard]] vec3 abs() const {
            return {std::abs(x), std::abs(y), std::abs(z)};
        }

        vec3& normalize() {
            auto len = length();
            if (len != 0) {
                x /= len;
                y /= len;
                z /= len;
            }
            return *this;
        }

        static vec3 normalize(vec3 v) {
            auto len = v.length();
//            if (len == 0) {
//                std::cerr << "length is zero, can't normalize" << "\n";
//                return {0, 0, 0};
//            }
            return {v.x / len, v.y / len, v.z / len};
        }

        vec3& add(const vec3 &a) {
            x += a.x;
            y += a.y;
            z += a.z;
            return *this;
        }

        vec3& add(const T &a) {
            x += a;
            y += a;
            z += a;
            return *this;
        }

        vec3& sub(const vec3 &a) {
            x -= a.x;
            y -= a.y;
            z -= a.z;
            return *this;
        }

        vec3& sub(const T &a) {
            x -= a;
            y -= a;
            z -= a;
            return *this;
        }

        vec3& multiply(const T &a) {
            x *= a;
            y *= a;
            z *= a;
            return *this;
        }

        vec3& multiply(const vec3 &other) {
            x *= other.x;
            y *= other.y;
            z *= other.z;
            return *this;
        }

        vec3& div(const T &a) {
            x /= a;
            y /= a;
            z /= a;
            return *this;
        }

        [[nodiscard]] static T dot(const vec3 &a, const vec3 &b) {
            return a.x * b.x + a.y * b.y + a.z * b.z;
        }

        [[nodiscard]] T dot(const vec3 &a) const {
            return x * a.x + y * a.y + z * a.z;
        }

        [[nodiscard]] T length() const {
            return std::sqrt(x * x + y * y + z * z);
        }

        [[nodiscard]] T length_sqr() const {
            return x * x + y * y + z * z;
        }

        [[nodiscard]] static T distance(const vec3& v1, const vec3& v2) {
            return std::sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y) + (v1.z - v2.z) * (v1.z - v2.z));
        }

        [[nodiscard]] T distance(const vec3& a) const {
            return std::sqrt((x - a.x) * (x - a.x) + (y - a.y) * (y - a.y) + (z - a.z) * (z - a.z));
        }

        static vec3 reflect(const vec3& v, const vec3& n) {
            return v - n * 2 * vec3::dot(v, n);
        }

        void reflect(const vec3 &n) {
            *this = *this - n * 2 * this->dot(n);
        }

        static vec3 refract(const vec3& v, const vec3& n, T eta) {
            auto cos_theta = fmin(dot(-v, n), 1.0);
            vec3 r_out_parallel = eta * (v + cos_theta * n);
            vec3 r_out_perp = -std::sqrt(fabs(1.0 - r_out_parallel.length_sqr())) * n;
            return r_out_parallel + r_out_perp;
        }

        void refract(const vec3& n, T eta) {
            auto cos_theta = fmin(dot(-*this, n), 1.0);
            vec3 r_out_parallel = (*this + n * cos_theta) * eta;
            vec3 r_out_perp = n * -std::sqrt(fabs(1.0 - r_out_parallel.length_sqr()));
            *this = r_out_parallel + r_out_perp;
        }

        vec3 sign() const {
            return vec3{
                    static_cast<T>((0. < x) - (x < 0.)),
                    static_cast<T>((0. < y) - (y < 0.)),
                    static_cast<T>((0. < z) - (z < 0.))
            };
        }

        static vec3 max(const vec3 &a, const vec3 &b) {
            return {
                    std::max(a.x, b.x),
                    std::max(a.y, b.y),
                    std::max(a.z, b.z)
            };
        }

        static vec3 min(const vec3 &a, const vec3 &b) {
            return {
                    std::min(a.x, b.x),
                    std::min(a.y, b.y),
                    std::min(a.z, b.z)
            };
        }

        [[nodiscard]] vec3 max(const vec3 &other) const {
            return {
                    std::max(x, other.x),
                    std::max(y, other.y),
                    std::max(z, other.z)
            };
        }

        [[nodiscard]] vec3 min(const vec3 &other) const {
            return {
                    std::min(x, other.x),
                    std::min(y, other.y),
                    std::min(z, other.z)
            };
        }

        [[nodiscard]] vec3 cross(const vec3 &other) const {
            return {
                    y * other.z - z * other.y,
                    z * other.x - x * other.z,
                    x * other.y - y * other.x
            };
        }

        inline static vec3 cross(const vec3 &v1, const vec3 &v2) {
            return {
                    v1.y * v2.z - v1.z * v2.y,
                    v1.z * v2.x - v1.x * v2.z,
                    v1.x * v2.y - v1.y * v2.x
            };
        }

        bool isZero() const {
            // Return true if the vector is close to zero in all dimensions.
            constexpr T s = 1e-4;
            return (fabs(x) < s) && (fabs(y) < s) && (fabs(z) < s);
        }

        vec3 operator+(const vec3 &other) const {
            return {x + other.x, y + other.y, z + other.z};
        }

        void operator=(const T &a) {
            x = a; y = a; z = a;
        }

        vec3 operator+(const T &other) const {
            return {x + other, y + other, z + other};
        }

        vec3 operator-(const vec3 &other) const {
            return {x - other.x, y - other.y, z - other.z};
        }

        vec3 operator-(const T &other) const {
            return {x - other, y - other, z - other};
        }

        vec3 operator-() const {
            return {-x, -y, -z};
        }

        vec3 operator/(const T &divider) const {
            return {x / divider, y / divider, z / divider};
        }

        vec3 operator/(const vec3 &divider) const {
            return {x / divider.x, y / divider.y, z / divider.z};
        }

        vec3 operator*(const T &multiplier) const {
            return {x * multiplier, y * multiplier, z * multiplier};
        }

        vec3 operator*(const vec3 &multiplier) const {
            return {x * multiplier.x, y * multiplier.y, z * multiplier.z};
        }

        bool operator<(const vec3& other) const {
            if (x != other.x) return x < other.x;
            if (y != other.y) return y < other.y;
            return z < other.z;
        }

        // overload subscript operator for non-const access
        double& operator[](int index) {
            switch (index) {
                case 0: return x;
                case 1: return y;
                case 2: return z;
                default: throw std::out_of_range("Index out of range for vec3");
            }
        }

        // overload subscript operator for const access
        const double& operator[](int index) const {
            switch (index) {
                case 0: return x;
                case 1: return y;
                case 2: return z;
                default: throw std::out_of_range("Index out of range for vec3");
            }
        }

        // Method to invert the components of the vector
        vec3 invert() const {
            if (x == 0 || y == 0 || z == 0) {
                throw std::runtime_error("Inversion error: Division by zero.");
            }
            return vec3(1 / x, 1 / y, 1 / z);
        }

        // In-place inversion of the vector
        void invert() {
            if (x == 0 || y == 0 || z == 0) {
                throw std::runtime_error("Inversion error: Division by zero.");
            }
            x = 1 / x;
            y = 1 / y;
            z = 1 / z;
        }

        double norm() const {
            return std::sqrt(x * x + y * y + z * z);
        }

        // Линейная интерполяция
        static vec3 mix(const vec3 &a, const vec3 &b, double factor) {
            return vec3(
                    a.x * (1.0 - factor) + b.x * factor,
                    a.y * (1.0 - factor) + b.y * factor,
                    a.z * (1.0 - factor) + b.z * factor
            );
        }
    };

    using vec3d = vec3<double>;

    template<typename T>
    std::mt19937_64 vec3<T>::engine = std::mt19937_64(std::random_device{}());

    template<typename T>
    std::uniform_real_distribution<double> vec3<T>::dist = std::uniform_real_distribution<double>(0.0, 1.0);

    template<typename T>
    const vec3<double> vec3<T>::ZERO = vec3<double>(0.0, 0.0, 0.0);

    template<typename T>
    const vec3<double> vec3<T>::ONES = vec3<double>(1.0, 1.0, 1.0);

    template<typename T>
    const vec3<double> vec3<T>::MAX = vec3<double>(std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
}

RN::vec3d operator/(double scalar, RN::vec3d const & vec);
RN::vec3d operator*(double scalar, RN::vec3d const & vec);

#endif //RAYON_CPP_CMAKE_VEC3_H
