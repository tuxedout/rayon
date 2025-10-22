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
 *      Vec3.cpp
 *
 */
#include "Vec3.h"
#include "CMS/CosLUT.h"
#include "CMS/SinLUT.h"

RN::vec3d operator/(double scalar, RN::vec3d const & vec) {
    return {scalar / vec.x, scalar / vec.y, scalar / vec.z};
}

RN::vec3d operator*(double scalar, RN::vec3d const & vec) {
    return {scalar * vec.x, scalar * vec.y, scalar * vec.z};
}

template<>
RN::vec3d RN::vec3d::random_unit_vector() {
    auto theta = RN::random() * 2 * RN::PI_D; // theta от 0 до 2pi
    auto phi = acos(2 * RN::random() - 1); // cos(phi) равномерно от -1 до 1

    auto x = RN::LUTs::sin(phi) * RN::LUTs::cos(theta);
    auto y = RN::LUTs::sin(phi) * RN::LUTs::sin(theta);
    auto z = RN::LUTs::cos(phi);

    return {x, y, z};
}

template<>
RN::vec3d RN::vec3d::random_cosine_direction() {
    auto r1 = RN::random();
    auto r2 = RN::random();
    auto z = sqrt(1 - r2);

    auto phi = 2 * RN::PI_D * r1;
    auto x = RN::LUTs::cos(phi) * sqrt(r2);
    auto y = RN::LUTs::sin(phi) * sqrt(r2);

    return vec3(x, y, z);
}