//
// Created by tux on 11.03.2024.
//

#include "SFSTracer.h"
#include <random>

thread_local static std::random_device rd;
thread_local static std::mt19937 gen(rd());
thread_local static std::uniform_real_distribution<float> dis(0.0, 1.0);

float rand01() {
    return dis(gen);
}

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

void reflect(RN::vec2d &i, const RN::vec2d &n) {
    double idotn2 = (i.x * n.x + i.y * n.y) * 2.0f;
    i.x = i.x - idotn2 * n.x;
    i.y = i.y - idotn2 * n.y;
}

bool refract(RN::vec2d &v, const RN::vec2d &n, double eta) {
    double cosi = -RN::vec2d::dot(n, v);
    double cost2 = 1.0f - eta * eta * (1.0f - cosi * cosi);
    RN::vec2d t = v * eta + n* (eta * cosi - sqrt(std::abs(cost2)));
    if (cost2 > 0.0f) {
        v = t;
        return true;
    } else {
        //v = glm::vec2(0.0f); // Total internal reflection
        return false;
    }
}

RN::vec3 beerLambert(const RN::vec3 &a, const double distance) {
    return {exp(-a.x * distance), exp(-a.y * distance), exp(-a.z * distance)};
}

static RN::vec3 BLACK(0.0f,0,0);

RN::vec3 RN::SFSTracer::trace(const RN::vec2d &p, RN::vec2d &dir, const RN::Scene &scene, int depth, const SDFLUT *lut, double &distance) {
    RN::vec3 srv_trace_res;

//    RN::vec2d tracing_point;

    RN::vec2d normal;

    vec2d  tracing_point = p;

    srv_trace_res = BLACK;

    int step = 0;

    //double distance = 0.0;

    const SFHitInfo *hit_info;

    double sign;

    while (step < max_steps && distance < max_distance) {
//        // !STAT!
//        traces_count++

        if (!scene.contains(tracing_point)) {
            return BLACK;
        }

        //scene.hit(tracing_point, hit_info);
        hit_info = lut->getValue(tracing_point);

        sign = hit_info->distance < 0.0f ? -1.0f : 1.0f;

        distance = distance + sign * hit_info->distance;

        //printf("dis: %f", t);
        step++;
        if (hit_info->distance*sign < epsilon) {
            srv_trace_res =  hit_info->item->material->emission * (float)(1.0 + distance*0.78 + distance*distance*0.45);

            if ((hit_info->item->material->reflectivity > 0.0 || hit_info->item->material->eta > 0.0) && depth < max_depth){
                normal = lut->normal(tracing_point);

                double refl = hit_info->item->material->reflectivity;

//                if (hit_info->item->material->eta > 0.0){
//                    RN::vec2 refracted_direction = dir;
//                    if (refract(refracted_direction, normal, sign < 0.0 ? hit_info->item->material->eta : 1.0f / hit_info->item->material->eta )){
//
//                        double cosi = -(dir.x * normal.x + dir.y * normal.y);
//
//                        double cost = -(refracted_direction.x * normal.x + refracted_direction.y * normal.y);
//
//                        refl = sign < 0.0f ? fresnel(cosi, cost, hit_info->item->material->eta, 1.0f) : fresnel(cosi, cost, 1.0f, hit_info->item->material->eta);
//                        refl = fmax(fmin(refl, 1.0f), 0.0f);
//
//                        //tracing_point =tracing_point - normal * (float)bias;
//                        tracing_point = p + refracted_direction * abs(hit_info->distance);
//
//                        srv_trace_res = srv_trace_res + trace(tracing_point, refracted_direction, scene, depth + 1, lut, distance) * (1.0f - refl);
//                    }else{
//                        refl = 1.0;
//                    }
//                }

                if (refl > 0.0) {
                    reflect(dir, normal);

                    //sampling_point += normal * (float)bias;
                    tracing_point = tracing_point + dir * (hit_info->distance * sign);

                    //srv_trace_res.x = normal.x;// srv_trace_res + trace(tracing_point, dir, scene, depth + 1, lut)  * refl;
                    //srv_trace_res.y = normal.y;///srv_trace_res + trace(tracing_point, dir, scene, depth + 1, lut)  * refl;
                    //srv_trace_res.z = hit_info->distance;///srv_trace_res + trace(tracing_point, dir, scene, depth + 1, lut)  * refl;
                    srv_trace_res = srv_trace_res + trace(tracing_point, dir, scene, depth + 1, lut, distance)  * refl;
                }
            }
//            // !STAT!
//            distance_meter += t;
//            hits_count++;

            return srv_trace_res * beerLambert(hit_info->item->material->absorption, distance);
        }

        tracing_point = tracing_point + dir*hit_info->distance;
    }


//    // !STAT!
//    distance_meter += t;
//    misses_count++;

    return BLACK;
}



RN::vec3 RN::SFSTracer::sample(const RN::vec2d &p, const RN::Scene &scene, SDFLUT *lut) {
    double a = 0.0;

     RN::vec2d direction(0.0f, 0.0f);
     RN::vec3 result_color(0.0f, 0.0f, 0.0f);
     RN::vec2d sampling_point(0.0f, 0.0f);

    for (int i = 0; i < samples_per_pixel; i++) {
        double rand_a = (rand01() - 0.5)*0.15;
        double rand_b = (rand01() - 0.5)*0.15;

        direction.x = cos_values[i] + rand_a;
        direction.y = sin_values[i] + rand_b;

        sampling_point.x = p.x;
        sampling_point.y = p.y;

        int depth = 0;
        double distance = 0.0;

        result_color = result_color + trace(sampling_point, direction, scene, depth, lut, distance);
    }

    result_color = result_color / samples_per_pixel;

    return result_color;
}

int RN::SFSTracer::getSamplesPerPixel() const {
    return samples_per_pixel;
}

double RN::SFSTracer::getStepAngle() const {
    return step_angle;
}

// !STAT!
int RN::SFSTracer::getTracesCount() const {
    return traces_count;
}

int RN::SFSTracer::getHitsCount() const {
    return hits_count;
}

float RN::SFSTracer::getDistanceMeter() const {
    return distance_meter;
}

int RN::SFSTracer::getMissesCount() const {
    return misses_count;
}

void RN::SFSTracer::resetStats() {
    hits_count = 0;
    traces_count = 0;
    distance_meter = 0.0;
    misses_count = 0;
}
// !STAT!