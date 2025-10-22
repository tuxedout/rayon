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
 *      SFTriangle
 *
 */
#ifndef RAYON_SFTRIANGLE_H
#define RAYON_SFTRIANGLE_H

#include "SFDrawable.h"

namespace RN {

    class SFTriangle : public SFDrawable {
    public:
        double width;
        double height;
        vec2d center;
        vec2d n = {0,0};

        SFTriangle(float _width, float _height, vec2d c, std::shared_ptr<Material> &mat): SFDrawable(mat), width(_width), height(_height), center(c){
            n = vec2(height, width / 2.0);
            n.normalize();
        }

        SFTriangle(const SFTriangle &other) : SFDrawable(other) {
            width = other.width;
            height = other.height;
            center = other.center;
            n = other.n;
        }

        [[nodiscard]] double distance(const RN::vec2d &p) const override {
            double p_x = center.x - p.x;
            double p_y = center.y - p.y;
            return fmax( abs(p_x)*n.x + p_y*n.y - (height*n.y), -p_y);
        }

        [[nodiscard]] SFDrawableType type() const override {return circle;}

        ~SFTriangle() = default;
    };

}


#endif //RAYON_SFTRIANGLE_H
