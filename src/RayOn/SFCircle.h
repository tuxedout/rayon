/*
 *
 *       Created by tux on 11.03.2024.
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
 *      SDFCircle
 *
 */
#ifndef RAYLIB_TEMPLATE_SFCIRCLE_H
#define RAYLIB_TEMPLATE_SFCIRCLE_H

#include "SFDrawable.h"

namespace RN {

    class SFCircle : public SFDrawable {
    public:
        float radius;
        vec2d center;

        SFCircle(float r, vec2d c, std::shared_ptr<Material> &mat): SFDrawable(mat), radius(r), center(c){}

        SFCircle(const SFCircle &other) : SFDrawable(other) {
            radius = other.radius;
            center = other.center;
        }

        [[nodiscard]] double distance(const RN::vec2d &p) const override {
            return vec2d::distance(p, center) - radius;
        }

        [[nodiscard]] SFDrawableType type() const override {return circle;}

        ~SFCircle() = default;
    };

}


#endif //RAYLIB_TEMPLATE_SFCIRCLE_H
