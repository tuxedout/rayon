/*
 *
 *       Created by tux on 28.03.2024.
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
 *      CheckeredTexture.h
 *
 */

#ifndef RAYON_CHECKERED_H
#define RAYON_CHECKERED_H

#include "SolidColorTexture.h"


    namespace RN::Textures {

        class CheckeredTexture: public SolidColorTexture {
        public:
            CheckeredTexture(): SolidColorTexture(vec3d(0, 0, 0)), color_value2(vec3d(1, 1, 1)), scale(1.0) {

            }

            CheckeredTexture(const vec3d &c1, const vec3d &c2, const double scale):
                    SolidColorTexture(c1), color_value2(c2), scale(scale){}

            vec3d color_value2;
            double scale;

            const vec3d color(const vec2d &uv, const vec3d &p) override {
                auto tmp_uv = uv * scale;

                int x = static_cast<int>(tmp_uv.x);
                int y = static_cast<int>(tmp_uv.y);

                return (x + y) % 2 ? color_value : color_value2;
            }
        };

    } // Textures
// RN

#endif //RAYON_CHECKERED_H
