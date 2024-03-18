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
 *      SFSTracer contains rendering/tracing/sampling functions
 *
 */
#ifndef RAYLIB_TEMPLATE_SFSTRACER_H
#define RAYLIB_TEMPLATE_SFSTRACER_H

#include <cmath>

#include "Scene.h"
#include "SFCircle.h"
#include "Vec2.h"
#include "Vec3.h"

#include "SDFLUT.h"

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

        // maximum ray length
        float max_distance = 1.0;
        // trace function limit of recursive calls
        int max_depth = 10;

        //
        double epsilon = 0.00001;

        // maximum steps per ray
        int max_steps = 10;

        // not used now
        double bias = 0.0001;

        // sample point: cast rays in each direction(directions count = samples_per_pixel)
        [[nodiscard]] vec3 sample(const vec2d &p, const Scene &scene, SDFLUT *lut);

        // trace ray from point in pointed by dir
        vec3 trace( vec2d &p, vec2d &dir, const Scene &scene, int depth, const SDFLUT *lut, double &distance, int &step);

        // set samples_per_pixel and update dep. things
        void setSamplesPerPixel(int s){
            samples_per_pixel = s;
            step_angle = RN_PI * 2 / samples_per_pixel;

            sin_values.clear();
            cos_values.clear();

            sin_values.resize(samples_per_pixel);
            cos_values.resize(samples_per_pixel);

            for (int i = 0; i < samples_per_pixel; ++i) {
                double angle = i * step_angle;
                sin_values[i] = std::sin(angle);
                cos_values[i] = std::cos(angle);
            }
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

        // angle between rays casted by sample
        double step_angle = 0;

        int samples_per_pixel = 16;

        // TODO: consider putting things in order
        RN::vec3 result_color;
        //RN::vec2d tracing_point;
        RN::vec2d sampling_point;
        RN::vec2d direction;

        /*
         *
         * precalculated values
         *
         */
        std::vector<double > sin_values;
        std::vector<double > cos_values;
    };
}


#endif //RAYLIB_TEMPLATE_SFSTRACER_H
