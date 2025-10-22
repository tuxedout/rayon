/*
 *
 *       Created by tux on 31.05.2024.
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
 *      ONB.h
 *
 */

#ifndef RAYON_CPP_CMAKE_ONB_H
#define RAYON_CPP_CMAKE_ONB_H

#include "../Vec3.h"

namespace RN {

        class ONB {
        public:
            ONB() {}

            explicit ONB(const vec3d& n) {
                build_from_w(n);
            }

            [[nodiscard]] vec3d u() const { return axis[0]; }
            [[nodiscard]] vec3d v() const { return axis[1]; }
            [[nodiscard]] vec3d w() const { return axis[2]; }

            [[nodiscard]] vec3d local(double a, double b, double c) const {
                return a * u() + b * v() + c * w();
            }

            [[nodiscard]] vec3d local(const vec3d& a) const {
                return a.x * u() + a.y * v() + a.z * w();
            }

//            void build_from_w(const vec3d& n) {
//                axis[2] = n;
//                vec3d a = (fabs(n.x) > 0.9) ? vec3d(0, 1, 0) : vec3d(1, 0, 0);
//                axis[1] = vec3d::normalize(vec3d::cross(n, a));
//                axis[0] = vec3d::cross(axis[2], axis[1]);
//            }

            void build_from_w(const vec3d& w) {
                vec3d unit_w = vec3d::normalize(w);
                vec3d a = (fabs(unit_w.x) > 0.9) ? vec3d(0,1,0) : vec3d(1,0,0);
                vec3d v = vec3d::normalize(vec3d::cross(unit_w, a));
                vec3d u = vec3d::cross(unit_w, v);
                axis[0] = u;
                axis[1] = v;
                axis[2] = unit_w;
            }

        private:
            vec3d axis[3];
        };
} // RN

#endif //RAYON_CPP_CMAKE_ONB_H
