/*
 *
 *       Created by tux on 23.03.2024.
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
 *      RTSampler.h - samples and traces pixels and rays
 *
 */

#ifndef RAYON_RTSAMPLER_H
#define RAYON_RTSAMPLER_H

#include <random>
#include <chrono>
#include "../Containers/RTScene.h"
#include "../../Vec2.h"
#include "../../Camera.h"
#include "../Containers/RTLightsList.h"
#include "../../Materials/RasterColorMaterial.h"
#include "../../Color.h"
#include <immintrin.h>

namespace RN {

    class RTSampler : public std::enable_shared_from_this<RTSampler> {
    public:
        explicit RTSampler(const Camera &c, const RTLightsList l): camera(c), _lights(l), _combined_pdf(ItemsPDF(_lights.children()), CosinePDF({})){

            _items_pdf = ItemsPDF(_lights.children());

            sqrt_samples = int(sqrt(samples_per_pixel));
            stratum_size = 1.0 / sqrt_samples;

            px_size_u = camera.pixelSizeU();
            px_size_v = camera.pixelSizeV();
        }

        // sample with simple stratification and antialiasing(randomly sample square samples_per_pixel times)
        // pixel coordinate p[0..image_width, 0..image_height] using scene
        [[nodiscard]] virtual vec3d sampleAAS(const vec2i &p, std::shared_ptr<RTScene> &scene, std::shared_ptr<RTLightsList> &lights) noexcept;

        // attempt to make iterative version
        RN::vec3d traceIterative2(const ray3 &ray, const std::shared_ptr<RTScene> &scene, const std::shared_ptr<RTLightsList> &lights) noexcept;

        // get random position inside tiny square used for AA
        [[nodiscard]] RN::vec3d randomSquareSample() const;

    protected:
        // TODO: remove unused physics
        static double fresnel(double cosi, double cost, double etai, double etat);
        static vec3d beerLambert(vec3d a, double d);


        static std::uniform_real_distribution<double> distribution;
        static std::uniform_real_distribution<double> distribution_m1p1; // -1, 1
        static std::random_device  generator;

        static double random() {
//            unsigned long long random_number;
//            if (_rdrand64_step(&random_number)) {
//                // Преобразуем случайное целое число в диапазоне от 0 до максимального значения unsigned long long
//                // в число типа double в диапазоне от 0 до 1
//                return static_cast<double>(random_number) / std::numeric_limits<unsigned long long>::max();
//            }

            return distribution(generator);
        }

        const Camera &camera;

        RTLightsList _lights;

        ItemsPDF _items_pdf;
        CombinedItemsCosinePDF _combined_pdf;

        // max amount of recursive trace calls
        int max_depth = 20;

        //
        int samples_per_pixel = 1;

        // stratification stuff
        int sqrt_samples;
        double stratum_size;

        vec3d px_size_u;
        vec3d px_size_v;

        RN::RTHitInfo tmp_hit;
        RN::RayMatInfo mat_res;

        vec3d accum = {0};
        vec3d attenuation = {1.0, 1.0, 1.0};
        ray3 current_ray = {};
    };

} // RN

#endif //RAYON_RTSAMPLER_H