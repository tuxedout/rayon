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
 *      Refracting.h
 *
 */

#ifndef RAYLIB_TEMPLATE_REFRACTING_H
#define RAYLIB_TEMPLATE_REFRACTING_H

#include "ReflectingMaterial.h"

namespace RN {
    namespace Materials {
        class RefractingMaterial: public ReflectingMaterial {
        public:
            RefractingMaterial(const vec3d &e): ReflectingMaterial(e) {

            }

            double eta = 1.5; // air

            // Schlick's approximation
            static double reflectance(double c, double r) {
                auto r0 = (1-r) / (1+r);
                r0 = r0*r0;
                return r0 + (1-r0)*pow((1 - c),5);
            }

            static double fresnel(double cosi, double cost, double etai, double etat) {
                double rs = (etat * cosi - etai * cost) / (etat * cosi + etai * cost);
                double rp = (etai * cosi - etat * cost) / (etai * cosi + etat * cost);
                return (rs * rs + rp * rp) * 0.5f;
            }

            RayMatInfo & proceed(ray3 &incidence, RN::RTHitInfo &hit_info, RayMatInfo &ray_mat_info,
                                 std::shared_ptr<RTScene> scene = nullptr,
                                 std::shared_ptr<RTItem> lights = nullptr, RTSampler *sampler = nullptr) const override {
                //auto scatter_direction = vec3::reflect(incidence.direction(),hit_info.normal) + vec3::random_unit_vector() * roughness;

                double refraction_ratio = hit_info.front_face ? (1.0/eta) : eta;

                //incidence.direction.normalize();

                double cos_t = fmin(vec3d::dot(-incidence.direction, hit_info.normal), 1.0);
                double sin_t = sqrt(1.0 - cos_t*cos_t);

                vec3 reflect_direction = incidence.direction;

                double lcl_reflect = reflectivity;// * reflectance(cos_t, refraction_ratio);
                //double lcl_reflect = reflectivity * reflectance(cos_t, refraction_ratio);
                bool reflects = refraction_ratio * sin_t > 1.0;

                if (reflects || reflectance(cos_t, refraction_ratio) > random() * 0.5) {
                    //lcl_reflect = 1.0;

                    reflect_direction.reflect(hit_info.normal);

                    reflect_direction = reflect_direction + vec3d::random_on_hemisphere(hit_info.normal) * roughness;

                    auto refl_ray = ray3(hit_info.point, reflect_direction);

                    ray_mat_info.color = color->color(hit_info.uv, hit_info.point)*(1.0 - lcl_reflect);
                    ray_mat_info.emitted = 0;
                    ray_mat_info.ray = refl_ray;
                    ray_mat_info.usable = true;
                    ray_mat_info.amount = lcl_reflect;
                    ray_mat_info.uses_pdf = false;

                    return ray_mat_info;
                }else{
                    // lcl_reflect = refraction_ratio;
                    reflect_direction.refract(hit_info.normal,refraction_ratio);

                    reflect_direction = reflect_direction + vec3d::random_on_hemisphere(hit_info.normal) * roughness;

                    auto refl_ray = ray3(hit_info.point, reflect_direction);

//                RayMatInfo result(hit_info.normal, refl_ray, false, 1.0);
                    //RayMatInfo result(color->color(hit_info.uv, hit_info.point)*(1.0 - lcl_reflect), refl_ray, true, lcl_reflect, std::make_shared<CosinePDF>(hit_info.normal),
                    //                  false);
                    ray_mat_info.color = color->color(hit_info.uv, hit_info.point)*(1.0 - lcl_reflect);
                    ray_mat_info.emitted = 0;
                    ray_mat_info.ray = refl_ray;
                    ray_mat_info.usable = true;
                    ray_mat_info.amount = lcl_reflect;
                    ray_mat_info.uses_pdf = false;

                    return ray_mat_info;
                }
            }

            [[nodiscard]] MaterialType type() const override {
                return mt_refracting;
            }

            double scatterPDF(const ray3 ray_in, RN::RTHitInfo &hit_info, const ray3 ray_out) const override {
                auto cos_theta = vec3d::dot(vec3d::normalize(hit_info.normal), vec3d::normalize(ray_out.direction));
                return cos_theta < 0 ? 0 : cos_theta/RN::PI_D;
            }
        };
    } // Materials
} // RN

#endif //RAYLIB_TEMPLATE_REFRACTING_H