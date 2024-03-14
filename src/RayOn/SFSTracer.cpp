//
// Created by root on 11.03.2024.
//

#include "SFSTracer.h"
#include <random>
#include <glm/gtc/noise.hpp>

float rand01() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(0.0, 1.0);
    return dis(gen);
}

float fresnel(float cosi, float cost, float etai, float etat) {
    float rs = (etat * cosi - etai * cost) / (etat * cosi + etai * cost);
    float rp = (etai * cosi - etat * cost) / (etai * cosi + etat * cost);
    return (rs * rs + rp * rp) * 0.5f;
}

float schlick(float cosi, float cost, float etai, float etat) {
    float r0 = (etai - etat) / (etai + etat);
    r0 *= r0;
    float a = 1.0f - (etai < etat ? cosi : cost);
    float aa = a * a;
    return r0 + (1.0f - r0) * aa * aa * a;
}

glm::vec2 reflect(const glm::vec2 &v, const glm::vec2 &n) {
    return v - 2.0f * glm::dot(v, n) * n;
}

bool refract(glm::vec2 &v, const glm::vec2 &n, float eta) {
    float cosi = -glm::dot(n, v);
    float cost2 = 1.0f - eta * eta * (1.0f - cosi * cosi);
    glm::vec2 t = eta * v + (eta * cosi - sqrt(abs(cost2))) * n;
    if (cost2 > 0.0f) {
        v = t;
        return true;
    } else {
        //v = glm::vec2(0.0f); // Total internal reflection
        return false;
    }
}

glm::vec3 beerLambert(const glm::vec3 &a, const float &distance) {
    return glm::vec3 (expf(-a.x * distance), expf(-a.y * distance), expf(-a.z * distance));
}

glm::vec3 BLACK(0,0,0);

glm::vec3 RN::SFSTracer::trace(glm::vec2 &p, glm::vec2 &dir, const RN::Scene &scene, int depth) const {
    int step = 0;

    float distance = 0.0;

    glm::vec3 srv_trace_res(0.0,0.0,0.0);

    SFHitInfo hit_info(0.0, nullptr);

    glm::vec2 sampling_point;

    float sign;

    while (step < max_steps && distance < max_distance) {
//        // !STAT!
//        traces_count++;
        sampling_point = p;

        scene.hit(sampling_point, hit_info);

        sign = hit_info.distance < 0.0f ? -1.0f : 1.0f;

        distance = distance + hit_info.distance;

        //printf("dis: %f", t);
        step++;

        if (hit_info.distance < epsilon) {

            srv_trace_res =  hit_info.item->material->emission;// / (float)(1.0 + distance*Ap + distance*distance*Ai);

            if ((hit_info.item->material->reflectivity > 0.0 || hit_info.item->material->eta > 0.0) && depth < max_depth){
                glm::vec2 normal = hit_info.item->normal(sampling_point);

                float refl = hit_info.item->material->reflectivity;

//                if (hit_info.item->material->eta > 0.0){
//                    glm::vec2 refracted_direction = dir;
//                    if (refract(refracted_direction, normal, sign < 0.0 ? hit_info.item->material->eta : 1.0f / hit_info.item->material->eta )){
//
//                        float cosi = -(dir.x * normal.x + dir.y * normal.y);
//
//                        float cost = -(refracted_direction.x * normal.x + refracted_direction.y * normal.y);
//
//                        refl = sign < 0.0f ? fresnel(cosi, cost, hit_info.item->material->eta, 1.0f) : fresnel(cosi, cost, 1.0f, hit_info.item->material->eta);
//                        refl = fmaxf(fminf(refl, 1.0f), 0.0f);
//
//                        sampling_point -= normal * (float)bias;
//                        //sampling_point = p + abs(hit_info.distance) * refracted_direction;
//
//                        srv_trace_res += (1.0f - refl) * trace(sampling_point, refracted_direction, scene, depth + 1);
//                    }else{
//                        refl = 1.0;
//                    }
//                }

                if (refl > 0.0) {
                    glm::vec2 reflected_direction = reflect(dir, normal);

                    //sampling_point += normal * (float)bias;
                    sampling_point = p + (hit_info.distance) * reflected_direction;

                    srv_trace_res += refl * trace(sampling_point, reflected_direction, scene, depth + 1);
                }
            }
//            // !STAT!
//            distance_meter += t;
//            hits_count++;

            return srv_trace_res * beerLambert(hit_info.item->material->absorption, distance);
        }

        p = p + dir*(hit_info.distance);
    }


//    // !STAT!
//    distance_meter += t;
//    misses_count++;

    return BLACK;
}

glm::vec3 RN::SFSTracer::sample(const glm::vec2 &p, const RN::Scene &scene) const {
    double a = 0.0;

    glm::vec2 sampling_point = p;

    glm::vec2 direction(0,0);

    glm::vec3 result_color(0.0,0.0,0.0);

    for (int i = 0; i < samples_per_pixel; i++) {
        double rand_a = (rand01() - 0.5);

        //float rand_a = (glm::perlin(p * 100.0f) - 0.5) * 0.2;

        direction.x = (float)std::cos(a + rand_a );
        direction.y = (float)std::sin(a + rand_a );

        sampling_point.x = p.x;
        sampling_point.y = p.y;

        int depth = 0.0;

        result_color += trace(sampling_point, direction, scene, depth);

        a += step_angle;
    }

    result_color /= samples_per_pixel;

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