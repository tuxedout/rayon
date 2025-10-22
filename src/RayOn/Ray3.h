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
 *      Ray3.h - 3D ray
 *
 */

#ifndef RAYON_RAY3_H
#define RAYON_RAY3_H

#include "Vec3.h"

namespace RN {

    class ray3 {
    public:
        ray3(): origin(0), direction({0,0,1}){

        }

        static ray3 ZERO(){
            return {vec3d(),vec3d()};
        }

        ray3(vec3d o, vec3d d): origin(o), direction(d) {}

        ray3(const ray3 &other) = default;

        ray3 operator+(const vec3d& v) const {
            return {origin + v, direction};
        }

        ray3 operator-(const vec3d& v) const {
            return {origin - v, direction};
        }

        void operator=(const double& v) {
            origin.x = v;
            origin.y = v;
            origin.z = v;

            direction.x = v;
            direction.y = v;
            direction.z = v;
        }

        [[nodiscard]] vec3d at(double distance) const{
            return origin + direction * distance;
        }

        vec3d origin;
        vec3d direction;
    };

} // RN

#endif //RAYON_RAY3_H
