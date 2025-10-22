/*
 *
 *       Created by tux on 28.04.2024.
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
 *      DispersiveMaterial.h
 *
 */

#ifndef RAYON_CPP_CMAKE_DISPERSIVEMATERIAL_H
#define RAYON_CPP_CMAKE_DISPERSIVEMATERIAL_H

#include "Material.h"
#include "../plus-d/RTHitInfo.h"

namespace RN {
    namespace Materials {
        class DispersiveMaterial : public Material {
        public:
            RayMatInfo & proceed(ray3 &incidence, RN::RTHitInfo &hit_info, RayMatInfo &ray_mat_info,
                                 std::shared_ptr<RTScene> scene = nullptr,
                                 std::shared_ptr<RTItem> lights = nullptr, RTSampler *sampler = nullptr) const override {
                RTHitInfo tmp_hit_info;

                vec3d color;

                ray3 tmp_ray(hit_info.point, {});

                for (auto wavelength : wavelengths) {
                    double refractiveIndex = getRefractiveIndex(wavelength);

                    vec3d refracted = incidence.direction;

                    refracted.refract(hit_info.normal, refractiveIndex);

                    tmp_ray.direction = refracted;

                    //color.add(sampler->trace(tmp_ray, scene, lights, 1));
                }

                color.div(wavelengths.size());

                RayMatInfo result(color/*color->color(hit_info.uv, hit_info.point)*/, tmp_ray, false, 1.0);
                return result;
            }

            [[nodiscard]] MaterialType type() const override {
                return mt_dispersive;
            }

        private:
            std::vector<double> wavelengths = {380, 450, 495, 570, 590, 620, 750};

            double getRefractiveIndex(double wavelength) const {
                // Simplistic example: linear interpolation or use a formula
                return 1.0 + (wavelength - 380) * 0.0005;
            }
        };
    }
} // RN

#endif //RAYON_CPP_CMAKE_DISPERSIVEMATERIAL_H
