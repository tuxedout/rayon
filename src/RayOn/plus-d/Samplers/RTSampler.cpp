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
 *      RTSampler.cpp
 *
 */

#include "RTSampler.h"
#include "../../CMS/PDF.h"
#include "../Containers/RTLightsList.h"

// init random related stuff, still not clear how to organize this in general
auto rtsmplr_seed = std::chrono::steady_clock::now().time_since_epoch().count();
std::uniform_real_distribution<double> RN::RTSampler::distribution(0.0, 1.0);
std::uniform_real_distribution<double> RN::RTSampler::distribution_m1p1(-1.0, 1.0);
std::random_device  RN::RTSampler::generator;

// used to clamp color values
static RN::Interval intensity(0.00001, 0.99999);

// generate anti-aliased & stratified sample
RN::vec3d RN::RTSampler::sampleAAS(const vec2i &p, std::shared_ptr<RTScene> &scene, std::shared_ptr<RTLightsList> &lights) noexcept{
    // Получаем центр пикселя в координатах сцены
    auto pixel_center = camera.pixelCenter(p);

    // Сумма выборок
    vec3d sum = {0};

    ray3 ray(camera.center(), {});

    for (int i = 0; i < sqrt_samples; ++i) {
        for (int j = 0; j < sqrt_samples; ++j) {
            // Выбираем случайную точку в каждом страте
            double px = (i + RN::random()) * stratum_size;
            double py = (j + RN::random()) * stratum_size;

            // Позиция образца пикселя с учётом смещения
            // Масштабируем смещение страта по размерам пикселя в направлениях U и V
            vec3d pixel_sample = pixel_center +
                                 px_size_u * (px - 0.5) +
                                 px_size_v * (py - 0.5);

            // Направление луча
            vec3d direction = pixel_sample.add(-camera.center());
            direction.normalize();

            // Луч из центра камеры к выбранному образцу пикселя
            ray.direction = direction;

            // Накапливаем цвет
            sum.add(traceIterative2(ray, scene, lights));
        }
    }

    // Усредняем значение цвета
    sum.div(samples_per_pixel);

    // Ограничиваем значения цвета, чтобы избежать переполнения
    sum.x = intensity.clamp(sum.x);
    sum.y = intensity.clamp(sum.y);
    sum.z = intensity.clamp(sum.z);

    return sum;
}

//thread_local static RN::RTHitInfo tmp_hit;
//thread_local static RN::RayMatInfo mat_res;

RN::vec3d RN::RTSampler::traceIterative2(const ray3 &ray,const std::shared_ptr<RTScene> &scene,const std::shared_ptr<RTLightsList> &lights) noexcept {
    accum = 0;
    attenuation = 1.0;
    current_ray = ray;
    int depth = max_depth;

    while (depth > 0) {

        if (!scene->hit(current_ray, 0.0001, std::numeric_limits<double>::infinity(), tmp_hit)) {
            // Если луч не попадает ни в один объект сцены, добавляем цвет фона
            accum.add(scene->background);
//            vec3 unit_direction = vec3d::normalize(current_ray.direction);
//            double t = 0.5 * (unit_direction.y + 1.0);
//            vec3d background_color = (1.0 - t) * vec3d(1.0, 1.0, 1.0) + t * vec3d(0.5, 0.7, 1.0);
//            accum.add(attenuation.multiply(background_color));
            return accum;
        }

        mat_res.color = 0;
        mat_res.emitted = 0;


        tmp_hit.mat->proceed(current_ray, tmp_hit, mat_res, scene, lights, this);

        if (!mat_res.usable) {
            accum.add(attenuation.multiply(mat_res.color.add(mat_res.emitted)));
            return accum;
        }

        if (mat_res.uses_pdf) {
            //_items_pdf.setOrigin(tmp_hit.point);

            //light_pdf.setOrigin(tmp_hit.point);
            //lights->pdf.setOrigin(tmp_hit.point);
//
//            CosinePDF cos_pdf(tmp_hit.normal);
//
//            CombinedItemsCosinePDF combined_pdf(_items_pdf, cos_pdf);

            _combined_pdf.items_PDF.setOrigin(tmp_hit.point);
            _combined_pdf.other_PDF.set(tmp_hit.normal);

            mat_res.ray.direction = _combined_pdf.items_PDF.generate(tmp_hit.normal);
            mat_res.ray.origin = tmp_hit.point;


            mat_res.ray.direction.normalize();

            double pdf = _combined_pdf.value(mat_res.ray.direction);

            //scattering_pdf = tmp_hit.mat->scatterPDF(ray, tmp_hit, mat_res.ray);
            double scattering_pdf = tmp_hit.mat->scatterPDF(current_ray, tmp_hit, mat_res.ray);

//            mat_res.color * scattering_pdf;
//            mat_res.color.div(pdf);
        }

        mat_res.ray.direction.normalize();

        // Обновляем затухание с учетом рассеянного цвета
        attenuation.multiply(mat_res.color);
        attenuation.add(mat_res.emitted);

        // Добавляем излучаемый цвет
        accum.multiply(attenuation);

        // Переход к следующему лучу
        current_ray = mat_res.ray;

        // Уменьшаем глубину
        depth--;
    }

    return accum;
}

RN::vec3d RN::RTSampler::randomSquareSample() const {
    auto px = -0.5 + random();
    auto py = -0.5 + random();
    return (camera.pixelSizeU() * px) + (camera.pixelSizeV() * py);
}

double RN::RTSampler::fresnel(double cosi, double cost, double etai, double etat) {
    double rs = (etat * cosi - etai * cost) / (etat * cosi + etai * cost);
    double rp = (etai * cosi - etat * cost) / (etai * cosi + etat * cost);
    return (rs * rs + rp * rp) * 0.5f;
}

RN::vec3d RN::RTSampler::beerLambert(RN::vec3d a, double d) {
    vec3 c = { exp(-a.x * d), exp(-a.y * d), exp(-a.z * d) };
    return c;
}