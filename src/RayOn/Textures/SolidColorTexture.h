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

#ifndef RAYON_SOLIDCOLOR_H
#define RAYON_SOLIDCOLOR_H

#include "Texture.h"

namespace RN::Textures {

    class SolidColorTexture: public Texture{
    public:
        explicit SolidColorTexture(const vec3d &c): color_value(c){};

        virtual const vec3d color(const vec2d &uv, const vec3d &p) override {
            return color_value;
        }

        vec3d color_value;
    };

} // RN

#endif //RAYON_SOLIDCOLOR_H
