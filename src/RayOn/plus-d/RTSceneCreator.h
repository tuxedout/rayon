/*
 *
 *       Created by tux on 15.04.2024.
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
 *      RTSceneCreator.h - creates and initializes scene
 *
 */

#ifndef RAYON_CPP_CMAKE_RTSCENECREATOR_H
#define RAYON_CPP_CMAKE_RTSCENECREATOR_H

#include "Containers/RTScene.h"
#include "../Camera.h"

namespace RN {

    class RTSceneCreator {
    public:
        virtual void initScene(RN::RTScene &scene, RN::RTScene &lights, Camera &camera) = 0;
    };

} // RN

#endif //RAYON_CPP_CMAKE_RTSCENECREATOR_H
