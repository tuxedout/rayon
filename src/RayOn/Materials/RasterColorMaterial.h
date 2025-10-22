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
 *      RasterColor.h
 *
 */

#ifndef RAYON_CPP_CMAKE_RASTERCOLORMATERIAL_H
#define RAYON_CPP_CMAKE_RASTERCOLORMATERIAL_H

#include "SolidColorMaterial.h"

namespace RN {
    namespace Materials {
        class RasterColorMaterial: public SolidColorMaterial {
        public:
            explicit RasterColorMaterial(const vec3d &c ): SolidColorMaterial(c) {}

            explicit RasterColorMaterial(const std::shared_ptr<Textures::Texture> &t): SolidColorMaterial(t){
                color = t;
            }

            RayMatInfo & proceed(ray3 &incidence, RN::RTHitInfo &hit_info, RayMatInfo &ray_mat_info,
                                 std::shared_ptr<RTScene> scene = nullptr,
                                 std::shared_ptr<RTItem> lights = nullptr, RTSampler *sampler = nullptr) const override;
        };

        class RasterColorMaterialIlluminated: public SolidColorMaterial {
        public:
            explicit RasterColorMaterialIlluminated(const vec3d &c ): SolidColorMaterial(c) {}

            explicit RasterColorMaterialIlluminated(const std::shared_ptr<Textures::Texture> &t): SolidColorMaterial(t){
                color = t;
            }

            RayMatInfo &proceed(ray3 &incidence, RN::RTHitInfo &hit_info, RayMatInfo &ray_mat_info,
                               std::shared_ptr<RTScene> scene = nullptr,
                               std::shared_ptr<RTItem> lights = nullptr, RTSampler *sampler = nullptr) const override;

        };

    } // Materials
} // RN

#endif //RAYON_CPP_CMAKE_RASTERCOLORMATERIAL_H
