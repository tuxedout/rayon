/*
 *
 *       Created by tux on 28.03.2024.
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
 *      SolidColorMaterial.cpp
 *
 */
#include "SolidColorMaterial.h"
#include "../plus-d/RTHitInfo.h"
#include "../CMS/ONB.h"
#include "../CMS/PDF.h"

RN::RayMatInfo
&RN::Materials::SolidColorMaterial::proceed(ray3 &incidence, RN::RTHitInfo &hit_info, RayMatInfo &ray_mat_info,
                                           std::shared_ptr<RTScene> scene,
                                           std::shared_ptr<RTItem> lights, RTSampler *sampler) const {

    ray_mat_info.color = color->color(hit_info.uv, hit_info.point);
    ray_mat_info.emitted = 0;
    ray_mat_info.ray = 0;
    ray_mat_info.usable = true;
    ray_mat_info.amount = 1.0;
    ray_mat_info.uses_pdf = true;
    ray_mat_info.ray.direction = vec3d::random_on_hemisphere(hit_info.normal);

    return ray_mat_info;
}

double RN::Materials::SolidColorMaterial::scatterPDF(const ray3 ray_in, RN::RTHitInfo &hit_info, const ray3 ray_out) const {
    auto cos_theta = vec3d::dot(hit_info.normal, ray_out.direction);
    return cos_theta;// < 0 ? 0 : cos_theta;
}