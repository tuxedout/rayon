/*
 *
 *       Created by tux on 30.03.2024.
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
 *      Diffusive.cpp
 *
 */
#include "DiffusiveMaterial.h"

#include "../plus-d/RTHitInfo.h"

RN::RayMatInfo &RN::Materials::DiffusiveMaterial::proceed(ray3 &incidence, RN::RTHitInfo &hit_info, RayMatInfo &ray_mat_info,
                                                           std::shared_ptr<RTScene> scene,
                                                           std::shared_ptr<RTItem> lights, RTSampler *sampler) const {
    vec3d emitted(0.0);

    if (hit_info.front_face) {
        emitted = emission->color(hit_info.uv, hit_info.point);
    }

    ray_mat_info.color = color->color(hit_info.uv, hit_info.point);
    ray_mat_info.emitted = emitted;
    ray_mat_info.ray = 0;
    ray_mat_info.usable = false;
    ray_mat_info.amount = reflectivity;
    ray_mat_info.uses_pdf = false;

    return ray_mat_info;
}
