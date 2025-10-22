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
 *      Material - quite self explanatory
 *
 */
#ifndef RAYON_MATERIAL_H
#define RAYON_MATERIAL_H


#include "../Vec3.h"
#include "../Ray3.h"

namespace RN {
    // forward declaration of ray-surface hit information
    struct RTHitInfo;

    // forward declarations of scene and sampler
    class RTScene;
    class RTItem;
    class RTSampler;
    class PDF;

    // ray-material interaction result
    class RayMatInfo {
    public:
        RayMatInfo(): color(0,0,0), ray(ray3::ZERO()), usable(false) {}

        // color, transformed ray, valid or not, transfered amount
        RayMatInfo(const vec3d &c, const ray3 &r, const bool u, const double a, std::shared_ptr<PDF> pdf = nullptr, bool uses_pdf = false):
                color(c), ray(r), usable(u), amount(a), pdf(pdf), uses_pdf(uses_pdf) {}

        RayMatInfo(const vec3d &c, const vec3d &e, const ray3 &r, const bool u, const double a, std::shared_ptr<PDF> pdf = nullptr, bool uses_pdf = false):
                color(c), emitted(e), ray(r), usable(u), amount(a), pdf(pdf), uses_pdf(uses_pdf) {}

        vec3d color = {0,0,0}; // resulting color of ray-material interaction
        vec3d emitted = {0,0,0}; // color emitted by material(if any)
        ray3 ray; // reflected or refracted ray
        bool usable; // is interaction result usable?

        bool uses_pdf = false;

        double amount = 0; // amount of light transfer [0..1]

        std::shared_ptr<PDF> pdf;
    };

    // all materials are going to be in RN::Materials namespace
    namespace Materials {

        // material types
        enum MaterialType {
            mt_material,
            mt_diffusive,
            mt_solidcolor,
            mt_refracting,
            mt_reflecting,
            mt_dispersive,
            mt_isotropic
        };

        class Material {
        public:
            Material() : ambient(0.0, 0.0, 0.0), diffuse(0.0, 0.0, 0.0), specular(0.0, 0.0, 0.0),
                         shininess(0.0), transparency(1.0), reflectivity(0.2) {}

            // reflectivity rate [0..1]
            double reflectivity;

            vec3d ambient;
            vec3d diffuse;
            vec3d specular;
            double shininess;
            double transparency;

            virtual RayMatInfo &proceed(ray3 &incidence, RN::RTHitInfo &hit_info, RayMatInfo &ray_mat_info,
                                         std::shared_ptr<RTScene> scene = nullptr,
                                         std::shared_ptr<RTItem> lights = nullptr, RTSampler *sampler = nullptr) const = 0;

            virtual double scatterPDF(const ray3 ray_in, RN::RTHitInfo &hit_info, const ray3 ray_out) const {
                return 0;
            }

            [[nodiscard]] virtual MaterialType type() const = 0;
        };
    }
} // RN::Materials


#endif // RAYON_MATERIAL_H
