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
 *      RTHitInfo.h
 *
 */

#ifndef RAYON_RTHITINFO_H
#define RAYON_RTHITINFO_H

#include "../Vec3.h"
#include "../Ray3.h"
#include "../Materials/Material.h"

namespace RN {

    class RTDrawable;

    struct RTHitInfo {
        std::shared_ptr<const RN::Materials::Material> mat;

        vec3d point;

        vec3d normal;

        double distance;

        bool front_face;

        vec2d uv;

        void set_face_normal(const ray3& r, const vec3d& outward_normal) {
            // Sets the hit record normal vector.
            // NOTE: the parameter `outward_normal` is assumed to have unit length.

            front_face = vec3d::dot(r.direction, outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }
    };

} // RN

#endif //RAYON_RTHITINFO_H
