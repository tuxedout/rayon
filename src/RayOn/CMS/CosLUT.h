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
 *      CosLUT.h - interpolated cosines LUT
 *
 */

#ifndef RAYON_CPP_CMAKE_COSLUT_H
#define RAYON_CPP_CMAKE_COSLUT_H

#include <cmath>
#include <vector>
#include "../RayOn.h"

namespace RN {
    class CosLUT {
    public:
        CosLUT(int pointsPerQuarterCircle) {
            // Create LUT that includes cos values at 0, π & 2π
            resolution = 4 * pointsPerQuarterCircle;
            lut.resize(resolution + 1);

            // Calculate and fill table
            for (int i = 0; i <= resolution; ++i) {
                double angle = 2 * RN::PI_D * i / resolution;
                lut[i] = std::cos(angle);
            }
        }

        double get(double x) const {
            //return std::cos(x);
            // normalize x to [0, 2*PI)
            x = fmod(x, 2 * RN::PI_D);
            if (x < 0) x += 2 * RN::PI_D;

            // calculate indexes and interpolate value
            double scaledX = x / (2 * RN::PI_D) * resolution;
            int index0 = static_cast<int>(scaledX);
            int index1 = (index0 + 1) % (resolution + 1);
            double frac = scaledX - index0;

            // linear interpolation
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
        extern const RN::CosLUT cos(360);
    }

} // RN

#endif //RAYON_CPP_CMAKE_COSLUT_H
