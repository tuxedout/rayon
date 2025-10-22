/*
 *
 *       Created by tux on 08.06.2024.
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
 *      SinLUT.h
 *
 */

#ifndef RAYON_CPP_CMAKE_SINLUT_H
#define RAYON_CPP_CMAKE_SINLUT_H

#include <cmath>
#include <vector>
#include "../RayOn.h"

namespace RN {

    class SinLUT {
    public:
        SinLUT(int pointsPerQuarterCircle) {
            // Create LUT that includes cos values at  0, π/2, π, 3π/2 и 2π
            resolution = 4 * pointsPerQuarterCircle; // 4 части по pointsPerQuarterCircle точек каждая
            lut.resize(resolution + 1);

            // Заполняем LUT значениями синуса
            for (int i = 0; i <= resolution; ++i) {
                double angle = 2 * RN::PI_D * i / resolution;
                lut[i] = std::sin(angle);
            }
        }

        double get(double x) const {
            //return std::sin(x);
            // Нормализация x в диапазон [0, 2*PI)
            x = fmod(x, 2 * RN::PI_D);
            if (x < 0) x += 2 * RN::PI_D;

            // Вычисление индексов и интерполяционного коэффициента
            double scaledX = x / (2 * RN::PI_D) * resolution;
            int index0 = static_cast<int>(scaledX);
            int index1 = (index0 + 1) % (resolution + 1);
            double frac = scaledX - index0;

            // Линейная интерполяция
            return lut[index0] * (1 - frac) + lut[index1] * frac;
        }

        double operator()(const double a) const {
            return get(a);
        }

    private:
        std::vector<double> lut;
        int resolution;
    };

    namespace LUTs{
        const RN::SinLUT sin(360);
    }

} // RN

#endif //RAYON_CPP_CMAKE_SINLUT_H
