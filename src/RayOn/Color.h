/*
 *
 *       Created by tux on 06.06.2024.
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
 *      Color.h
 *
 */

#ifndef RAYON_CPP_CMAKE_COLOR_H
#define RAYON_CPP_CMAKE_COLOR_H

#include "Vec3.h"

namespace RN {

    namespace Color {
        const vec3d Black = {0,0,0};
        const vec3d White = {1,1,1};

        const vec3d Red =   {1,0,0};
        const vec3d Green = {0,1,0};
        const vec3d Blue =  {0,0,1};

        const vec3d Yellow =    {1,1,0};
        const vec3d Orange =    {1,0.25,0};
        const vec3d Pink =      {1,0,1};
        const vec3d Cyan =      {0,1,1};
        const vec3d LightBlue = {0,0.67,1};


    };

} // RN

#endif //RAYON_CPP_CMAKE_COLOR_H
