/*
 *
 *       Created by tux on 09.06.2024.
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
 *      RasterColor.cpp
 *
 */

#include "RasterColorMaterial.h"
#include "../plus-d/RTHitInfo.h"

namespace RN {
    namespace Materials {
        RayMatInfo & RasterColorMaterial::proceed(ray3 &incidence, RN::RTHitInfo &hit_info, RayMatInfo &ray_mat_info,
                                                  std::shared_ptr<RTScene> scene,
                                                  std::shared_ptr<RTItem> lights, RTSampler *sampler) const {

            ray_mat_info.color = color->color(hit_info.uv, hit_info.point) * incidence.direction.dot(-hit_info.normal);
            ray_mat_info.emitted = {incidence.direction.dot(-hit_info.normal)*0};
            ray_mat_info.ray = {};
            ray_mat_info.usable = false;
            ray_mat_info.amount = 1.0;

            return ray_mat_info;
        }
    } // Materials
} // RN