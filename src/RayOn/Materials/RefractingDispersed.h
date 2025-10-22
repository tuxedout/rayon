/*
 *
 *       Created by tux on 23.04.2024.
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
 *      RefractingDispersed.h
 *
 */

#ifndef RAYON_CPP_CMAKE_REFRACTINGDISPERSED_H
#define RAYON_CPP_CMAKE_REFRACTINGDISPERSED_H

#include "RefractingMaterial.h"

namespace RN {
    namespace Materials {
        class RefractingDispersed: public RefractingMaterial {
        public:
            RefractingDispersed(const vec3d &e): RefractingMaterial(e) {

            }

            vec3d eta = {1.0};

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

            RayMatInfo proceed(ray3 &incidence, RTHitInfo &hit_info) const final {
                //auto scatter_direction = vec3::reflect(incidence.direction(),hit_info.normal) + vec3::random_unit_vector() * roughness;

                auto refraction_ratio = hit_info.front_face ? (1.0/eta.y) : eta.y;

                //incidence.direction.normalize();

                double cos_t = fmin(vec3d::dot(-incidence.direction, hit_info.normal), 1.0);
                double sin_t = sqrt(1.0 - cos_t*cos_t);

                vec3 reflect_direction = incidence.direction;

                double lcl_reflect = reflectivity;// * reflectance(cos_t, refraction_ratio);
                //double lcl_reflect = reflectivity * reflectance(cos_t, refraction_ratio);
                bool reflects = refraction_ratio * sin_t > 1.0;

                if (reflects || reflectance(cos_t, refraction_ratio) > random()) {
                    //lcl_reflect = 1.0;

                    reflect_direction.reflect(hit_info.normal);
                }else{
                    // lcl_reflect = refraction_ratio;
                    reflect_direction = refract(reflect_direction, hit_info.normal);
                    reflect_direction.normalize();
                }


                //lcl_transmit -= lcl_reflect;

                reflect_direction = reflect_direction + vec3d::random_on_hemisphere(hit_info.normal) * roughness;

                auto refl_ray = ray3(hit_info.point, reflect_direction);

//                RayMatInfo result(hit_info.normal, refl_ray, false, 1.0);
                RayMatInfo result(color->color(hit_info.uv, hit_info.point), refl_ray, true, lcl_reflect);
                return result;
            }

            vec3d refract(const vec3d& incident, const vec3d& normal) const {
                vec3d refractedRed = vec3d::refract(incident, normal, eta.x);
                vec3d refractedGreen = vec3d::refract(incident, normal, eta.y);
                vec3d refractedBlue = vec3d::refract(incident, normal, eta.z);

                return vec3d(refractedRed.x, refractedGreen.y, refractedBlue.z); // Простой способ смешивания компонент
            }
        };
    }

} // RN

#endif //RAYON_CPP_CMAKE_REFRACTINGDISPERSED_H
