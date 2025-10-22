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
 *      vec2 represents 2D
 *
 */

#ifndef RAYON_CPP_CMAKE_VEC2_H
#define RAYON_CPP_CMAKE_VEC2_H

#include <cstdint>
#include <cmath>

namespace RN {
    template<typename T>
    struct vec2 {
    public:
        T x, y;
        vec2(T _x, T _y) :
                x(_x),
                y(_y) {}

        vec2(const vec2 &other) = default;


        vec2(): x(0), y(0) {}

        [[nodiscard]] inline vec2 abs() const {
            return {std::abs(x), std::abs(y)};
        }

        [[nodiscard]] inline static vec2 abs(const vec2 v) {
            return {std::abs(v.x), std::abs(v.y)};
        }

        inline vec2 &normalize() {
            auto len = length();
            if (len == 0) return *this;
            x = ((x) / len);
            y = ((y) / len);

            return *this;
        }

        inline vec2 &operator+=(const vec2 &other) const {
            x += other.x;
            y += other.y;

            return *this;
        }

        inline vec2 &operator+=(const double &other) const {
            x += other;
            y += other;

            return *this;
        }

        inline vec2 &operator*=(const double &other) const {
            x *= other;
            y *= other;

            return *this;
        }

        inline vec2 operator+(const vec2& other) const {
            return {(x + other.x), (y + other.y) };
        }

        inline vec2 operator+(const float &other) const {
            return {x + (other), y + (other) };
        }

        inline vec2 operator-(const vec2& other) const {
            return {(x - other.x), (y - other.y) };
        }

        inline vec2 operator-(const float &other) const {
            return {x - (other), y - (other) };
        }

        inline vec2 operator/(const T &divider) const {
            return {x / (divider), y / (divider) };
        }

        inline vec2 operator*(const T &multiplier) const {
            return {(x * (multiplier)), (y * (multiplier)) };
        }

        bool operator==(const vec2& other) const {
            return x == other.x && y == other.y;
        }

        inline void add(const vec2 &a) {
            x += a.x;
            y += a.y;
        }

        inline void add(const T &a) {
            x += (a) ;
            y += (a);
        }

        inline void sub(const vec2 &a) {
            x -= a.x;
            y -= a.y;
        }

        inline void sub(const T &a) {
            x -= (a);
            y -= (a);
        }

        inline vec2<double> multiply(const T &a) {
            x *= (a);
            y *= (a);

            return *this;
        }

        vec2 *div(const T &a) {
            x /= (a);
            y /= (a);

            return *this;
        }

        [[nodiscard]] inline static T dot(const vec2 &a, const vec2 &b){
            return ((a.x)*b.x + (a.y)*b.y);
        }

        [[nodiscard]] inline T dot(const vec2 &a) const{
            return ((a.x)*x + (a.y)*y);
        }

        [[nodiscard]] inline T length() const {
            auto lengthSquared = (x ) * (x ) + (y ) * (y );

            return std::sqrt(lengthSquared);
        }

        [[nodiscard]] static T distance(const vec2& a, const vec2& b) {
            T dx = a.x - b.x;
            T dy = a.y - b.y;

            T distSquared = dx * dx + dy * dy ;

            return std::sqrt(distSquared);
        }

        [[nodiscard]] T distance(const vec2& a) const {
            T dx = x - a.x;
            T dy = y - a.y;

            T distSquared = dx * dx + dy * dy ;

            return std::sqrt(distSquared);
        }

        inline void max(const T &param) {
            x = x > param ? x : param;
            y = y > param ? y : param;
        }

        inline static void reflect(vec2 &i, const vec2 &n) {
            double idotn2 = (i.x * n.x + i.y * n.y) * 2.0f;
            i.x = i.x - idotn2 * n.x;
            i.y = i.y - idotn2 * n.y;
        }
    };

    using vec2f = vec2<float>;
    using vec2i = vec2<int>;
    using vec2d = vec2<double>;
}

#endif //RAYON_CPP_CMAKE_VEC2_H
