/*
 *
 *       Created by tux on 25.03.2024.
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
 *      RayOn.h
 *
 */

#ifndef RAYON_RAYON_H
#define RAYON_RAYON_H

#include <chrono>
#include <random>

namespace RN {
    extern const double PI_D;
    extern const double PI_PI_D;
    extern const double EPSILON_D;
    extern const double EPSILON_100_D;

    extern std::uniform_real_distribution<double> _rn_distribution;
    extern std::random_device  _rn_generator;

    double random();
    double random(const double &min, const double &max);
    int random_int(int min, int max);
}

#endif //RAYON_RAYON_H

