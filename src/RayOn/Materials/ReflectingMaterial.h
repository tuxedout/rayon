/*
 *
 *       Created by tux on 24.03.2024.
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
 *      Reflecting.h - reflecting material
 *
 */

#ifndef RAYON_REFLECTINGMATERIAL_H
#define RAYON_REFLECTINGMATERIAL_H

#include "SolidColorMaterial.h"
#include "../plus-d/RTHitInfo.h"
#include <immintrin.h>

namespace RN::Materials {

class ReflectingMaterial: public SolidColorMaterial {
    public:
        // random generation related stuff
        // engine
        static std::random_device engine;
        // distribution that defines interval(def: (0..1)
        static std::uniform_real_distribution<double> dist;

        // how much light is being reflected from 0(no reflection at all) to 1(full reflection)
        double reflectivity = 0.0;

        ReflectingMaterial(const vec3d& color): SolidColorMaterial(color) {
        }


        RayMatInfo &proceed(ray3 &incidence, RN::RTHitInfo &hit_info, RayMatInfo &ray_mat_info,
                           std::shared_ptr<RTScene> scene = nullptr,
                           std::shared_ptr<RTItem> lights = nullptr, RTSampler *sampler = nullptr) const override {
            vec3 reflect_direction = vec3d::reflect(incidence.direction,hit_info.normal) + vec3d::random_unit_vector() * roughness;

            double lcl_reflect = reflectivity;// * reflectance(cos_t, refraction_ratio);

            auto refl_ray = ray3(hit_info.point, reflect_direction);

            RayMatInfo result(color->color(hit_info.uv, hit_info.point), refl_ray, true, lcl_reflect);

            ray_mat_info.color = color->color(hit_info.uv, hit_info.point);
            ray_mat_info.emitted = 0;
            ray_mat_info.ray = refl_ray;
            ray_mat_info.usable = true;
            ray_mat_info.amount = reflectivity;
            ray_mat_info.uses_pdf = false;
            ray_mat_info.pdf = nullptr;

            return ray_mat_info;
        }

        virtual double scatterPDF(const ray3 ray_in, RN::RTHitInfo &hit_info, const ray3 ray_out) const override {
            return 1 / (4 * RN::PI_D);
        }

        static double random() {
//            unsigned long long random_number;
//            if (_rdrand64_step(&random_number)) {
//                // Преобразуем случайное целое число в диапазоне от 0 до максимального значения unsigned long long
//                // в число типа double в диапазоне от 0 до 1
//                return static_cast<double>(random_number) / std::numeric_limits<unsigned long long>::max();
//            } else {
//                throw std::runtime_error("Ошибка генерации случайного числа");
//            }

            return dist(engine);
        }

        [[nodiscard]] MaterialType type() const override {
            return mt_reflecting;
        }
    };

} // RN

#endif //RAYON_REFLECTINGMATERIAL_H
