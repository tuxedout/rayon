/*
 *
 *       Created by tux on 11.03.2024.
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
 *      SolidColorMaterial - quite self explanatory
 *
 */
#ifndef RAYON_SOLIDCOLOR_MATERIAL_H
#define RAYON_SOLIDCOLOR_MATERIAL_H

#include "../Vec3.h"
#include "../Ray3.h"
#include "Material.h"
#include "../Textures/Texture.h"
#include "../Textures/SolidColorTexture.h"

namespace RN::Materials {
    // simple solid color material
    class SolidColorMaterial: public Material{
    public:
        explicit SolidColorMaterial(const vec3d &c){
            color = std::make_shared<Textures::SolidColorTexture>(c);
        }

        explicit SolidColorMaterial(const std::shared_ptr<Textures::Texture> &t){
            color = t;
        }

        RayMatInfo &proceed(ray3 &incidence, RN::RTHitInfo &hit_info, RayMatInfo &ray_mat_info,
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
}

#endif //RAYON_SOLIDCOLOR_MATERIAL_H