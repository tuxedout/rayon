//
// Created by root on 11.03.2024.
//

#ifndef RAYLIB_TEMPLATE_SFSTRACER_H
#define RAYLIB_TEMPLATE_SFSTRACER_H

#include "Scene.h"
#include "SFCircle.h"
#include <cmath>

const double RN_PI = 3.1415926535897932384626433832795;

namespace RN {
    class SFSTracer {
    public:
        SFSTracer() {
            setSamplesPerPixel(samples_per_pixel);
        }

        SFSTracer(const SFSTracer &tracer){
            max_depth = tracer.max_depth;
            max_distance = tracer.max_distance;
            max_steps = tracer.max_steps;
            epsilon = tracer.epsilon;

            setSamplesPerPixel(tracer.getSamplesPerPixel());
        }

//        glm::vec2 direction;

        float max_distance = 1.0;
        int max_depth = 10;
        double epsilon = 0.000001;
        int max_steps = 10;
        double bias = 0.0001;

        [[nodiscard]] glm::vec3 sample(const glm::vec2 &p, const Scene &scene) const;

        glm::vec3 trace(glm::vec2 &p, glm::vec2 &dir, const Scene &scene, int depth) const;

        void setSamplesPerPixel(int s){
            samples_per_pixel = s;
            step_angle = RN_PI * 2 / samples_per_pixel;
        }

        [[nodiscard]] int getSamplesPerPixel() const;
        [[nodiscard]] double getStepAngle() const;

        // !STAT!
        [[nodiscard]] int getTracesCount() const;
        [[nodiscard]] int getHitsCount() const;
        [[nodiscard]] int getMissesCount() const;
        [[nodiscard]] float getDistanceMeter() const;

        void resetStats();
        // !STAT!
    private:
        // !STAT!
        int traces_count = 0;
        int hits_count = 0;
        int misses_count = 0;
        float distance_meter = 0.0;
        // !STAT!

        double step_angle = 0;
        int samples_per_pixel = 16;
        //glm::vec3 result_color;
    };
}


#endif //RAYLIB_TEMPLATE_SFSTRACER_H
