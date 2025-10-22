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
#ifndef RAYON_SFPOININFO_H
#define RAYON_SFPOININFO_H

#include "Materials/Material.h"
#include "SFDrawable.h"
#include <memory>

namespace RN {
    class SFPoinInfo {
    public:
        SFPoinInfo(): distance(0), distance2(0), item(nullptr), item2(nullptr) {};
        SFPoinInfo(double d, SFDrawable *i): distance(d),  distance2(0), item(i),item2(nullptr) {};

        SFPoinInfo(const SFPoinInfo &hi) = default;

        // distance to nearest object
        double distance;
        double distance2;

        // pointer to nearest object
        SFDrawable *item;
        SFDrawable *item2;
    };
}


#endif //RAYON_SFPOININFO_H
