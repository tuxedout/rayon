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
 *      Diffusive.h - diffusive light material, emits color
 *
 */

#ifndef RAYON_CPP_CMAKE_DIFFUSIVEMATERIAL_H
#define RAYON_CPP_CMAKE_DIFFUSIVEMATERIAL_H

#include "Material.h"
#include "../Textures/Texture.h"
#include "../Textures/SolidColorTexture.h"

namespace RN {
    namespace Materials {
        class DiffusiveMaterial: public Material{
        public:
            explicit DiffusiveMaterial(const vec3d &emission): emission(std::make_shared<Textures::SolidColorTexture>(emission)), color(std::make_shared<Textures::SolidColorTexture>(vec3(1.0, 1.0, 1.0))) {

            }

            explicit DiffusiveMaterial(const vec3d &emission, const vec3d &color): emission(std::make_shared<Textures::SolidColorTexture>(emission)), color(std::make_shared<Textures::SolidColorTexture>(color)) {

            }

            explicit DiffusiveMaterial(std::shared_ptr<Textures::Texture> e, std::shared_ptr<Textures::Texture> c): emission(e), color(c) {

            }

            [[nodiscard]] MaterialType type() const override {
                return mt_diffusive;
            }

            // proceed incidence ray
            RayMatInfo & proceed(ray3 &incidence, RN::RTHitInfo &hit_info, RayMatInfo &ray_mat_info,
                                 std::shared_ptr<RTScene> scene = nullptr,
                                 std::shared_ptr<RTItem> lights = nullptr, RTSampler *sampler = nullptr) const override;
        private:
            // color of emitted light
            std::shared_ptr<Textures::Texture> emission;

            // color of material
            std::shared_ptr<Textures::Texture> color;
        };

    } // Materials
} // RN

#endif //RAYON_CPP_CMAKE_DIFFUSIVEMATERIAL_H
