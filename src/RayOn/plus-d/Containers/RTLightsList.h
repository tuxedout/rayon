/*
 *
 *       Created by tux on 23.03.2024.
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
 *      RTScene.h
 *
 */

#ifndef RAYON_RTLIGHTSLIST_H
#define RAYON_RTLIGHTSLIST_H

#include <vector>
#include <memory>
#include "RTScene.h"
#include "../../CMS/PDF.h"

namespace RN {

    class ItemsPDF;

    class RTLightsList: public RTScene {
    public:
        RTLightsList(): RTScene(){
            pdf = ItemsPDF();
        }

        [[nodiscard]] RTItemType type() const override {return rt_lightslist;}

        ItemsPDF pdf;
    };

} // RN

#endif //RAYON_RTLIGHTSLIST_H
