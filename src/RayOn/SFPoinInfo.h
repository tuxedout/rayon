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
 *      SFPoinInfo.h - contains info about distance field point
 *
 */
#ifndef RAYLIB_TEMPLATE_SFPOININFO_H
#define RAYLIB_TEMPLATE_SFPOININFO_H

#include "Material.h"
#include "SFDrawable.h"
#include <memory>

namespace RN {
    class SFPoinInfo {
    public:
        SFPoinInfo(): distance(0), item(nullptr){};
        SFPoinInfo(double d, SFDrawable *i): distance(d), item(i){};

        SFPoinInfo(const SFPoinInfo &hi) = default;

        // distance to nearest object
        double distance;

        // pointer to nearest object
        SFDrawable *item;
    };
}


#endif //RAYLIB_TEMPLATE_SFPOININFO_H
