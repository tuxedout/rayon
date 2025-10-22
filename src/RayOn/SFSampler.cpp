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
 *      SFSampler contains rendering/tracing/sampling functions
 *
 */
#include "SFSampler.h"
#include "SDFLUT.h"

// TODO: remove mess and unuseds
double fresnel(double cosi, double cost, double etai, double etat) {
    double rs = (etat * cosi - etai * cost) / (etat * cosi + etai * cost);
    double rp = (etai * cosi - etat * cost) / (etai * cosi + etat * cost);
    return (rs * rs + rp * rp) * 0.5f;
}

double schlick(double cosi, double cost, double etai, double etat) {
    double r0 = (etai - etat) / (etai + etat);
    r0 *= r0;
    double a = 1.0f - (etai < etat ? cosi : cost);
    double aa = a * a;
    return r0 + (1.0f - r0) * aa * aa * a;
}

//RN::vec2d reflect(const RN::vec2d &v, const RN::vec2d &n) {
//    return v - n * 2.0f * v.dot( n);
//}

// TODO: replace with faster approximation
RN::vec3 beerLambert(const RN::vec3 &a, const double distance) {
    return {exp(a.x * distance), exp(a.y * distance), exp(a.z * distance)};
}

static RN::vec3 BLACK(0.0f,0,0);


//RN::vec3 RN::SFSampler::trace(RN::vec2d &p, RN::vec2d &dir, const RN::SFScene &scene, int depth, const SDFLUT *lut, double &distance, int &step) {
//
//    RN::vec3 srv_trace_res = BLACK;
//
//    RN::vec2d normal;
//
//    RN::vec2d refracted_dir;
//    RN::vec2d reflected_dir;
//
//    const SFPoinInfo *hit_info;
//
//    double sign;
//
//    double color_divider = 0;
//
//    while (step < max_steps && distance < max_distance) {
////        // !STAT!
////        traces_count++
//
//        hit_info = lut->getSDFValue(p);
//
//        sign = hit_info->distance < 0.0f ? -1.0f : 1.0f;
//
//        distance = distance + sign * hit_info->distance;
//
//        double jump_distance = hit_info->distance < 0 ? hit_info->distance2 : hit_info->distance;
//
//        step++;
//
//        if (hit_info->distance < epsilon) {
//
//            if ((hit_info->item->material->reflectivity > 0.0 || hit_info->item->material->eta > 0.0) && depth < max_depth){
//                normal = lut->normal(p);
//
//                double refl = hit_info->item->material->reflectivity;
//
//                if (hit_info->item->material->eta > 0.0){
//                    double eta = 1 / hit_info->item->material->eta;
//                    double cosi = -normal.dot(dir);
//                    double k = 1.0 - eta * eta * (1.0 - cosi * cosi);
//                    if (k < 0) { // Total internal reflection condition
//                        refl = 1;
//                    } else {
//                        // Handle refraction
//                        refracted_dir =  dir * eta + normal * (eta * cosi - sqrt(k)) ; // Refraction direction
//
//                        //RN::vec2d refracted_origin = p - normal * (sign * bias); // Offset origin for refraction
//                        RN::vec2d refracted_origin = p + refracted_dir * jump_distance; // Offset origin for refraction
//
//                        srv_trace_res = srv_trace_res + (hit_info->item->material->emission * beerLambert(hit_info->item->material->absorption, distance) + trace(refracted_origin, refracted_dir, scene, depth + 1, lut, distance, step)) * (1.0 - refl);
//                        //srv_trace_res.add((hit_info->item->material->emission + trace(refracted_origin, refracted_dir, scene, depth + 1, lut, distance, step)).multiply(1.0 - refl));
//
//
//                        color_divider++;
//                    }
//                }
//
//                if (refl > 0.0) {
//                    vec2d ::reflect(dir, normal);
//
//                    //p = p + dir * (jump_distance);
//                    //p = p + dir * hit_info->distance * sign;
//
//                    srv_trace_res = srv_trace_res + hit_info->item->material->emission  * refl;
//                    //srv_trace_res = srv_trace_res + (hit_info->item->material->emission + trace(p, dir, scene, depth + 1, lut, distance, step))  * refl;
//
//                    color_divider++;
//                }
//            }
////            // !STAT!
////            distance_meter += t;
////            hits_count++;
//            color_divider = std::max(1.0,color_divider);
//            return srv_trace_res / color_divider;//*beerLambert(hit_info->item->material->absorption, distance);
//        }
//
//        p = p + dir * hit_info->distance * sign;
//        //p = p + dir * jump_distance;
//    }
//
//
////    // !STAT!
////    distance_meter += t;
////    misses_count++;
//    color_divider = std::max(1.0,color_divider);
//
//    return srv_trace_res / color_divider;
//}

RN::vec3 RN::SFSampler::sample(const RN::vec2d &p, const RN::SFScene &scene, const SDFLUT *lut ) {
    result_color = {0.0,0.0,0.0};

    //return result_color;

    for (int i = 0; i < samples_per_pixel; i++) {
        vec2d noise = lut->getNoiseValue(p)*0.04;

        direction.x = cos_values[i] + noise.x;
        direction.y = sin_values[i] + noise.y;

        //direction.normalize();

        sampling_point.x = p.x;
        sampling_point.y = p.y;

        int depth = 0;
        int step  = 0;

        double distance = 0.0;

        result_color = result_color + trace(sampling_point, direction, scene, depth, lut, distance, step);
        //return trace(sampling_point, direction, scene, depth, lut, distance, step);
    }

    //result_color = result_color;

    result_color.x = result_color.x / samples_per_pixel;
    result_color.y = result_color.y / samples_per_pixel;
    result_color.z = result_color.z / samples_per_pixel;

    return result_color;
}

RN::vec3 RN::SFSampler::sampleSDF(const RN::vec2d &p, const RN::SFScene &scene, const SDFLUT *lut) {
    result_color = {0.0,0.0,0.0};

    //return result_color;

    double sdf = lut->getDistanceValue(p);

    result_color.x = sdf;
    result_color.y = sdf;
    result_color.z = sdf;

    return result_color;
}

RN::vec3 RN::SFSampler::sampleNormales(const RN::vec2d &p, const RN::SFScene &scene, const RN::SDFLUT *lut) {
    result_color = {0.0,0.0,0.0};

    //return result_color;

    vec2d l_normal = lut->normal(p);

    result_color.x = l_normal.x;
    result_color.y = l_normal.y;
    result_color.z = 0.0;

    return result_color;
}

int RN::SFSampler::getSamplesPerPixel() const {
    return samples_per_pixel;
}

double RN::SFSampler::getStepAngle() const {
    return step_angle;
}

// !STAT!
int RN::SFSampler::getTracesCount() const {
    return traces_count;
}

int RN::SFSampler::getHitsCount() const {
    return hits_count;
}

float RN::SFSampler::getDistanceMeter() const {
    return distance_meter;
}

int RN::SFSampler::getMissesCount() const {
    return misses_count;
}

void RN::SFSampler::resetStats() {
    hits_count = 0;
    traces_count = 0;
    distance_meter = 0.0;
    misses_count = 0;
}
// !STAT!