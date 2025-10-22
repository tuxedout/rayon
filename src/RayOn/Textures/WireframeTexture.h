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
 *      SolidColorTexture.h - solid color texture
 *
 */

#ifndef RAYON_WIREFRAMETEXTURE_H
#define RAYON_WIREFRAMETEXTURE_H

#include "Texture.h"

namespace RN::Textures {

    class WireframeTexture: public Texture{
    public:
        explicit WireframeTexture(const vec3d &c): color_value(c){};

        virtual const vec3d color(const vec2d &uv, const vec3d &p) override {

            const double padding = 0.1;

            if (((uv.x < padding) || (uv.x > (1.0 - padding))) || ((uv.y < padding) || (uv.y > (1.0 - padding)))) {
                return color_value;
            }

            return {1.0};
        }

        vec3d color_value;
    };

} // RN

#endif //RAYON_SOLIDCOLOR_H