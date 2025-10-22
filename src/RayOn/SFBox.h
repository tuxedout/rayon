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
 *      SDFBox
 *
 */
#ifndef RAYON_SFBOX_H
#define RAYON_SFBOX_H

#include "SFDrawable.h"

namespace RN {

    class SFBox: public SFDrawable {
    public:

        SFBox(vec2d c, vec2d s, std::shared_ptr<Material> &mat) : SFDrawable(mat), center(c), size(s) {

        }

        SFBox(const SFBox &other): SFDrawable(other){
            center = other.center;
            size = other.size;
        }

        vec2d size;
        vec2d center;

        [[nodiscard]] double distance(const vec2d &p) const override {

            RN::vec2d srv_d;

            srv_d = (center - p).abs() - size;

            double min_max = std::fmin(std::fmax(srv_d.x, srv_d.y), 0.0);

            srv_d.max(0);

            return srv_d.length() + min_max;
        }

        [[nodiscard]] SFDrawableType type() const override {return box;}

        ~SFBox() = default;
    };

} // RN

#endif //RAYON_SFBOX_H
