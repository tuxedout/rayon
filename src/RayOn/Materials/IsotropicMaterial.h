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
 *      IsotropicMaterial.h
 *
 */

#ifndef RAYON_CPP_CMAKE_ISOTROPICMATERIAL_H
#define RAYON_CPP_CMAKE_ISOTROPICMATERIAL_H

#include "Material.h"
#include "../Textures/Texture.h"
#include "../Textures/SolidColorTexture.h"

namespace RN {
    namespace Materials {
        class IsotropicMaterial: public Material {
            public:
                explicit IsotropicMaterial(const vec3d &c){
                    color = std::make_shared<Textures::SolidColorTexture>(c);
                }

                explicit IsotropicMaterial(const std::shared_ptr<Textures::Texture> &t){
                    color = t;
                }

                RayMatInfo & proceed(ray3 &incidence, RN::RTHitInfo &hit_info, RayMatInfo &ray_mat_info,
                                     std::shared_ptr<RTScene> scene = nullptr,
                                     std::shared_ptr<RTItem> lights = nullptr, RTSampler *sampler = nullptr) const override;

                double scatterPDF(const ray3 ray_in, RN::RTHitInfo &hit_info, const ray3 ray_out) const override;

                // roughness of material [0..1]
                double roughness = 0.0;

                [[nodiscard]] MaterialType type() const override {
                    return mt_solidcolor;
                }

            protected:
                /*
                 * storing color as texture allows to use complex and generative color variations,
                 * so it can be any kind of texture(solid color, generative, image etc.)
                 */
                std::shared_ptr<RN::Textures::Texture> color;
        };
    } // Materials
} // RN

#endif //RAYON_CPP_CMAKE_ISOTROPICMATERIAL_H
