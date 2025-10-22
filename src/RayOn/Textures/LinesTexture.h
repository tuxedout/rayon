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

#ifndef RAYON_LINES_TEXTURE_H
#define RAYON_LINES_TEXTURE_H

#include "CheckeredTexture.h"


    namespace RN::Textures {

        class LinesTexture: public CheckeredTexture {
        public:
            LinesTexture(const vec3d &c1, const vec3d &c2, const double scale, bool flip):
                    CheckeredTexture(c1, c2, scale), rotated(flip){}

            /* rotate 90 deg. */
            bool rotated = true;

            const vec3d color(const vec2d &uv, const vec3d &p) override {
                auto tmp_uv = uv * scale;

                int x = static_cast<int>(tmp_uv.x);
                int y = static_cast<int>(tmp_uv.y);

                return (rotated ? y : x) % 2 ? color_value : color_value2;
            }
        };

    } // Textures
// RN

#endif //RAYON_LINES_TEXTURE_H
