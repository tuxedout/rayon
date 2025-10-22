/*
 *
 *       Created by tux on 08.06.2024.
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
 *      ColorMaterial.h
 *
 */

#ifndef RAYON_CPP_CMAKE_DEBUGMATERIALS_H
#define RAYON_CPP_CMAKE_DEBUGMATERIALS_H

#include "SolidColorMaterial.h"
#include "RefractingMaterial.h"

namespace RN {
    namespace Materials {

        class ColorMaterial: public SolidColorMaterial {
        public:
            explicit ColorMaterial(const vec3d &c ): SolidColorMaterial(c) {}

            explicit ColorMaterial(const std::shared_ptr<Textures::Texture> &t): SolidColorMaterial(t){
                color = t;
            }

            RayMatInfo &proceed(ray3 &incidence, RN::RTHitInfo &hit_info, RayMatInfo &ray_mat_info,
                               std::shared_ptr<RTScene> scene = nullptr,
                               std::shared_ptr<RTItem> lights = nullptr, RTSampler *sampler = nullptr) const override;
        };

        class NormalMaterial: public SolidColorMaterial {
        public:
            explicit NormalMaterial(): SolidColorMaterial(RN::vec3d::ZERO) {}

            RayMatInfo &proceed(ray3 &incidence, RN::RTHitInfo &hit_info, RayMatInfo &ray_mat_info,
                               std::shared_ptr<RTScene> scene = nullptr,
                               std::shared_ptr<RTItem> lights = nullptr, RTSampler *sampler = nullptr) const override;
        };

        class DepthMaterial: public SolidColorMaterial {
        public:
            explicit DepthMaterial(): SolidColorMaterial(RN::vec3d::ZERO) {}

            RayMatInfo &proceed(ray3 &incidence, RN::RTHitInfo &hit_info, RayMatInfo &ray_mat_info,
                                std::shared_ptr<RTScene> scene = nullptr,
                                std::shared_ptr<RTItem> lights = nullptr, RTSampler *sampler = nullptr) const override;
        };

        class IridescentMaterial: public RefractingMaterial {
        public:
            explicit IridescentMaterial(): RefractingMaterial(RN::vec3d::ZERO) {}

            RayMatInfo &proceed(ray3 &incidence, RN::RTHitInfo &hit_info, RayMatInfo &ray_mat_info,
                                std::shared_ptr<RTScene> scene = nullptr,
                                std::shared_ptr<RTItem> lights = nullptr, RTSampler *sampler = nullptr) const override;

            vec3d iridescentColor(double angle) const;
        };

        class IridescentRefractingMaterial: public IridescentMaterial{
        public:
            explicit IridescentRefractingMaterial(): IridescentMaterial() {}

            RayMatInfo &proceed(ray3 &incidence, RN::RTHitInfo &hit_info, RayMatInfo &ray_mat_info,
                                std::shared_ptr<RTScene> scene = nullptr,
                                std::shared_ptr<RTItem> lights = nullptr, RTSampler *sampler = nullptr) const override;
        };

    } // Materials
} // RN

#endif //RAYON_CPP_CMAKE_DEBUGMATERIALS_H
