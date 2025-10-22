/*
 *
 *       Created by tux on 02.06.2024.
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
 *      IsotropicMaterial.cpp
 *
 */

#include "IsotropicMaterial.h"

#include "../plus-d/RTHitInfo.h"
#include "../CMS/PDF.h"

namespace RN {
    namespace Materials {

        RayMatInfo &
        IsotropicMaterial::proceed(ray3 &incidence, RN::RTHitInfo &hit_info, RayMatInfo &ray_mat_info,
                                   std::shared_ptr<RTScene> scene,
                                   std::shared_ptr<RTItem> lights, RTSampler *sampler) const {
            RayMatInfo result(color->color(hit_info.uv, hit_info.point), {}, true, reflectivity, std::make_shared<SpherePDF>(), true);

            return result;
        }

        double IsotropicMaterial::scatterPDF(const ray3 ray_in, RTHitInfo &hit_info, const ray3 ray_out) const {
            return 1 / (4 * RN::PI_D);
        }
    } // Materials
} // RN