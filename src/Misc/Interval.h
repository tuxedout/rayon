/*
 *
 *       Created by tux on 14.03.2024.
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
 *      provides value that can be changed step by step within defined limits
 *
 */

#ifndef RAYON_CPP_CMAKE_INTERVAL_H
#define RAYON_CPP_CMAKE_INTERVAL_H

namespace RN {
    template<typename T>
    class Interval {
    public:
        Interval(T v, T mi, T ma, T st): value(v), min_value(mi), max_value(ma), value_step(st){}

        T getValue() const {
            return value;
        }

        T stepUp() {
            value = (value + value_step <= max_value) ? value + value_step : max_value;
            return value;
        }

        T stepUp(const int steps) {
            value = (value + value_step * steps <= max_value) ? value + value_step * steps : max_value;
            return value;
        }

        T stepDown() {
            value = (value - value_step >= min_value) ? value - value_step : min_value;
            return value;
        }

        T stepDown(const int steps) {
            value = (value - value_step * steps >= min_value) ? value - value_step * steps : min_value;
            return value;
        }

        void setValue(T v) {
            value = v;
        }
    private:
        T value;
        T min_value;
        T max_value;
        T value_step;
    };

} // RN

#endif //RAYON_CPP_CMAKE_INTERVAL_H
