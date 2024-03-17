//
// Created by tux on 14.03.2024.
//

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
                x((_x)),
                y((_y)) {}

        vec2(const vec2 &other) = default;


        vec2(): x(0), y(0) {}

        [[nodiscard]] vec2 abs() const {
            return {std::abs(x), std::abs(y)};
        }

        [[nodiscard]] static vec2 abs(const vec2 v) {
            return {std::abs(v.x), std::abs(v.y)};
        }

        void normalize() {
            auto len = length();
            if (len == 0) return;
            x = ((x) / len);
            y = ((y) / len);
        }

        vec2 operator+(const vec2& other) const {
            return {(x + other.x), (y + other.y) };
        }

        vec2 operator+(const float &other) const {
            return {x + (other), y + (other) };
        }

        vec2 operator-(const vec2& other) const {
            return {(x - other.x), (y - other.y) };
        }

        vec2 operator-(const float &other) const {
            return {x - (other), y - (other) };
        }

        vec2 operator/(const T &divider) const {
            return {x / (divider), y / (divider) };
        }

        vec2 operator*(const T &multiplier) const {
            return {(x * (multiplier)), (y * (multiplier)) };
        }

        bool operator==(const vec2& other) const {
            return x == other.x && y == other.y;
        }

        void add(const vec2 &a) {
            x += a.x;
            y += a.y;
        }

        void add(const T &a) {
            x += (a) ;
            y += (a);
        }

        void sub(const vec2 &a) {
            x -= a.x;
            y -= a.y;
        }

        void sub(const T &a) {
            x -= (a);
            y -= (a);
        }

        void multiply(const T &a) {
            x *= (a);
            y *= (a);
        }

        void div(const T &a) {
            x /= (a);
            y /= (a);
        }

        [[nodiscard]] static T dot(const vec2 &a, const vec2 &b){
            return ((a.x)*b.x + (a.y)*b.y);
        }

        [[nodiscard]] T dot(const vec2 &a) const{
            return ((a.x)*x + (a.y)*y);
        }

        [[nodiscard]] T length() const {
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
    };

    using vec2f = vec2<float>;
    using vec2i = vec2<int>;
    using vec2d = vec2<double>;
}

#endif //RAYON_CPP_CMAKE_VEC2_H
