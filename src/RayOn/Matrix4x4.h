/*
 *
 *       Created by tux on 25.04.2024.
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
 *      Matrix4x4.h
 *
 */

#ifndef RAYON_CPP_CMAKE_MATRIX4X4_H
#define RAYON_CPP_CMAKE_MATRIX4X4_H
#include <array>
#include <cmath>
#include "vec3.h"

namespace RN {
    class Matrix4x4 {
    private:
        std::array<std::array<double, 4>, 4> m;

    public:
        Matrix4x4() {
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    m[i][j] = (i == j ? 1.0 : 0.0);  // Identity matrix by default
        }

        Matrix4x4(std::initializer_list<double> list) {
            auto it = list.begin();
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    m[i][j] = *it++;
        }

        double& operator()(int row, int col) {
            return m[row][col];
        }

        const double& operator()(int row, int col) const {
            return m[row][col];
        }

        static Matrix4x4 rotateX(double angle) {
            Matrix4x4 result;
            result(1,1) = cos(angle);
            result(1,2) = -sin(angle);
            result(2,1) = sin(angle);
            result(2,2) = cos(angle);
            return result;
        }

        static Matrix4x4 rotateY(double angle) {
            Matrix4x4 result;
            result(0,0) = cos(angle);
            result(0,2) = sin(angle);
            result(2,0) = -sin(angle);
            result(2,2) = cos(angle);
            return result;
        }

        static Matrix4x4 rotateZ(double angle) {
            Matrix4x4 result;
            result(0,0) = cos(angle);
            result(0,1) = -sin(angle);
            result(1,0) = sin(angle);
            result(1,1) = cos(angle);
            return result;
        }

        Matrix4x4 operator*(const Matrix4x4& other) const {
            Matrix4x4 result;
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j) {
                    result.m[i][j] = 0;
                    for (int k = 0; k < 4; ++k)
                        result.m[i][j] += m[i][k] * other.m[k][j];
                }
            return result;
        }

        vec3d operator*(const vec3d& vec) const {
            double x = vec.x * m[0][0] + vec.y * m[0][1] + vec.z * m[0][2] + m[0][3];
            double y = vec.x * m[1][0] + vec.y * m[1][1] + vec.z * m[1][2] + m[1][3];
            double z = vec.x * m[2][0] + vec.y * m[2][1] + vec.z * m[2][2] + m[2][3];
            double w = vec.x * m[3][0] + vec.y * m[3][1] + vec.z * m[3][2] + m[3][3];
            if (w == 1.0) return vec3d(x, y, z);
            else return vec3d(x/w, y/w, z/w);
        }

        Matrix4x4 transpose() const {
            Matrix4x4 result;
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    result.m[j][i] = m[i][j];
            return result;
        }

        Matrix4x4 inverse() const {
            // Это работает только для ортонормированных матриц вращения/отражения без масштабирования.
            // Такая матрица обратна своей транспонированной матрице.
            return transpose();
        }

        vec3d transformVector(const vec3d& vec) const {
            // Transform vector ignoring translation (last column of the matrix)
            double x = vec.x * m[0][0] + vec.y * m[0][1] + vec.z * m[0][2];
            double y = vec.x * m[1][0] + vec.y * m[1][1] + vec.z * m[1][2];
            double z = vec.x * m[2][0] + vec.y * m[2][1] + vec.z * m[2][2];
            return vec3d(x, y, z);
        }
    };
} // RN

#endif //RAYON_CPP_CMAKE_MATRIX4X4_H
