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
 *      ColorMaterial.cpp
 *
 */

#include "DebugMaterials.h"
#include "../plus-d/RTHitInfo.h"

namespace RN {
    namespace Materials {
        RayMatInfo &ColorMaterial::proceed(ray3 &incidence, RN::RTHitInfo &hit_info, RayMatInfo &ray_mat_info,
                                          std::shared_ptr<RTScene> scene,
                                          std::shared_ptr<RTItem> lights, RTSampler *sampler) const {

            ray_mat_info.color = color->color(hit_info.uv, hit_info.point);
            ray_mat_info.emitted = color->color(hit_info.uv, hit_info.point);
            ray_mat_info.ray = 0;
            ray_mat_info.usable = false;
            ray_mat_info.amount = 0;
            ray_mat_info.uses_pdf = false;

            return ray_mat_info;
        }

        RayMatInfo &NormalMaterial::proceed(ray3 &incidence, RN::RTHitInfo &hit_info, RayMatInfo &ray_mat_info,
                                           std::shared_ptr<RTScene> scene,
                                           std::shared_ptr<RTItem> lights, RTSampler *sampler) const {

            ray_mat_info.color = hit_info.normal;
            ray_mat_info.emitted = 0;
            ray_mat_info.ray = 0;
            ray_mat_info.usable = false;
            ray_mat_info.amount = 0;
            ray_mat_info.uses_pdf = false;

            return ray_mat_info;
        }

        RayMatInfo &DepthMaterial::proceed(ray3 &incidence, RN::RTHitInfo &hit_info, RayMatInfo &ray_mat_info,
                                          std::shared_ptr<RTScene> scene,
                                          std::shared_ptr<RTItem> lights, RTSampler *sampler) const {

            ray_mat_info.color = hit_info.distance;
            ray_mat_info.emitted = 0;
            ray_mat_info.ray = 0;
            ray_mat_info.usable = false;
            ray_mat_info.amount = 0;
            ray_mat_info.uses_pdf = false;

            return ray_mat_info;
        }

        RayMatInfo &IridescentMaterial::proceed(ray3 &incidence, RTHitInfo &hit_info, RayMatInfo &ray_mat_info,
                                                std::shared_ptr<RTScene> scene, std::shared_ptr<RTItem> lights,
                                                RTSampler *sampler) const {

            double lcl_cos = vec3d::dot(incidence.direction.normalize(), hit_info.normal.normalize());

            double angle = acos(lcl_cos);

            ray_mat_info.color   = iridescentColor(angle);

            ray_mat_info.emitted = 0;
            ray_mat_info.ray = 0;
            ray_mat_info.usable = false;
            ray_mat_info.amount = 0;
            ray_mat_info.uses_pdf = false;

            return ray_mat_info;
        }

        vec3d IridescentMaterial::iridescentColor(double angle) const {
            // Перламутровые цвета
            vec3d colors[5] = {
                    vec3d(1.0, 0.0, 0.0), // Red
                    vec3d(1.0, 0.7, 0.0), // Orange
                    vec3d(1.0, 0.3, 0.6), // Синий
                    vec3d(1.0, 1.0, 0.0), // Желтый
                    vec3d(1.0, 0.0, 1.0)  // Фиолетовый
            };

            double factor = angle / (RN::PI_D);
            int idx = int(factor * 5.0);
            double mixFactor = std::fmod(factor * 5.0, 1.0);

            return vec3d::mix(colors[idx % 5], colors[(idx + 1) % 5], mixFactor);
        }

        RayMatInfo &
        IridescentRefractingMaterial::proceed(ray3 &incidence, RTHitInfo &hit_info, RayMatInfo &ray_mat_info,
                                              std::shared_ptr<RTScene> scene, std::shared_ptr<RTItem> lights,
                                              RTSampler *sampler) const {
            double refraction_ratio = hit_info.front_face ? (1.0/eta) : eta;

            //incidence.direction.normalize();

            double cos_t = fmin(vec3d::dot(-incidence.direction, hit_info.normal), 1.0);
            double sin_t = sqrt(1.0 - cos_t*cos_t);

            vec3 reflect_direction = incidence.direction;

            double lcl_reflect = reflectivity;// * reflectance(cos_t, refraction_ratio);
            //double lcl_reflect = reflectivity * reflectance(cos_t, refraction_ratio);
            bool reflects = refraction_ratio * sin_t > 1.0;

            double lcl_cos = vec3d::dot(incidence.direction.normalize(), hit_info.normal);

            double angle = acos(lcl_cos);

            ray_mat_info.color   = iridescentColor(angle);

            if (reflects || reflectance(cos_t, refraction_ratio) > random() * 0.5) {
                //lcl_reflect = 1.0;

                reflect_direction.reflect(hit_info.normal);

                reflect_direction = reflect_direction + vec3d::random_on_hemisphere(hit_info.normal) * roughness;

                auto refl_ray = ray3(hit_info.point, reflect_direction);

                //ray_mat_info.color = color->color(hit_info.uv, hit_info.point)*(1.0 - lcl_reflect);
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
                //ray_mat_info.color = color->color(hit_info.uv, hit_info.point)*(1.0 - lcl_reflect);
                ray_mat_info.emitted = 0;
                ray_mat_info.ray = refl_ray;
                ray_mat_info.usable = true;
                ray_mat_info.amount = lcl_reflect;
                ray_mat_info.uses_pdf = false;

                return ray_mat_info;
            }
        }
    } // Materials
} // RN