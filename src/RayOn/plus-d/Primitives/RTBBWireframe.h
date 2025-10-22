/*
 *
 *       Created by tux on 08.06.2024.
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
 *      RTBBWireframe.h is not working because of RTLine fuckup
 *
 */

#ifndef RAYON_CPP_CMAKE_RTBBWIREFRAME_H
#define RAYON_CPP_CMAKE_RTBBWIREFRAME_H

#include "../RTDrawable.h"
#include "RTLine.h"
#include <vector>

namespace RN {

    class RTBBWireframe: public RTDrawable {
    public:
        RTBBWireframe(const vec3d &min, const vec3d &max, const std::shared_ptr<RN::Materials::Material> &mat)
                : RTDrawable(mat), _min(min), _max(max) {
            initialize();
        }

        bool hit(const ray3 &ray, double dmin, double dmax, RTHitInfo &hit_info) const override {
            for (const auto &line : _lines) {
                if (line->hit(ray, dmin, dmax, hit_info)) {
                    return true;
                }
            }
            return false;
        }

        double pdfValue(const vec3d &origin, const vec3d &direction) const override {
            return 0.0; // Not relevant for bounding box wireframe display
        }

        vec3d random(const vec3d &origin) const override {
            return vec3d(); // Not relevant for bounding box wireframe display
        }

        [[nodiscard]] RTItemType type() const override { return rt_bounding_box_wireframe; }

        [[nodiscard]] const AABB &bbox() const override {
            return _bbox;
        };

    private:
        void initialize() {
            vec3d vertices[8] = {
                    _min,
                    vec3d(_max.x, _min.y, _min.z),
                    vec3d(_max.x, _max.y, _min.z),
                    vec3d(_min.x, _max.y, _min.z),
                    vec3d(_min.x, _min.y, _max.z),
                    vec3d(_max.x, _min.y, _max.z),
                    vec3d(_max.x, _max.y, _max.z),
                    vec3d(_min.x, _max.y, _max.z)
            };

            addLine(vertices[0], vertices[1]);
            addLine(vertices[1], vertices[2]);
            addLine(vertices[2], vertices[3]);
            addLine(vertices[3], vertices[0]);

            addLine(vertices[4], vertices[5]);
            addLine(vertices[5], vertices[6]);
            addLine(vertices[6], vertices[7]);
            addLine(vertices[7], vertices[4]);

            addLine(vertices[0], vertices[4]);
            addLine(vertices[1], vertices[5]);
            addLine(vertices[2], vertices[6]);
            addLine(vertices[3], vertices[7]);

            _bbox = AABB(_min, _max);
        }

        void addLine(const vec3d &start, const vec3d &end) {
            _lines.push_back(std::make_shared<RTLine>(start, end, 0.3, material));
        }

        vec3d _min;
        vec3d _max;
        AABB _bbox;
        std::vector<std::shared_ptr<RTLine>> _lines;
    };
} // RN

#endif //RAYON_CPP_CMAKE_RTBBWIREFRAME_H
