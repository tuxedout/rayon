/*
 *
 *       Created by tux on 27.05.2024.
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
 *      RTScale.h - scales RTItem in world space by a given factor
 *
 */
#ifndef RAYON_CPP_CMAKE_RTSCALE_H
#define RAYON_CPP_CMAKE_RTSCALE_H

#include "../RTDrawable.h"

namespace RN::Transforms {

    class RTScale: public RTItem {
    public:
        RTScale(std::shared_ptr<RTItem> obj, const vec3d& scale_factors) : RTItem() {
            _item = obj;
            _scale_factors = scale_factors;

            _bbox = _item->bbox();

            vec3d min( std::numeric_limits<double>::infinity(),  std::numeric_limits<double>::infinity(),  std::numeric_limits<double>::infinity());
            vec3d max(-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity());

            for (int i = 0; i < 8; i++) {
                vec3d point = _bbox[i];
                vec3d scaled_point = scalePoint(point);

                min = vec3d::min(min, scaled_point);
                max = vec3d::max(max, scaled_point);
            }

            _bbox = AABB(min, max);
        }

        bool hit(const ray3 &ray, double dmin, double dmax, RTHitInfo &hit_info) const override {
            auto origin = scalePoint(ray.origin, false);
            auto direction = scalePoint(ray.direction, false);

            ray3 scaled_r(origin, direction);

            if (!_item->hit(scaled_r, dmin, dmax, hit_info))
                return false;

            hit_info.point = scalePoint(hit_info.point);
            hit_info.normal = scalePoint(hit_info.normal).normalize();

            return true;
        }

        const AABB &bbox() const override {
            return _bbox;
        }

        RTItemType type() const override {
            return rt_scale;
        }

        double pdfValue(const vec3d &origin, const vec3d &direction) const override {
            auto lcl_origin = scalePoint(origin, false);
            auto lcl_direction = scalePoint(direction, false);

            return _item->pdfValue(lcl_origin, lcl_direction);
        }

        vec3d random(const vec3d &origin) const override {
            auto lcl_origin = scalePoint(origin, false);

            return _item->random(lcl_origin);
        }
    private:
        std::shared_ptr<RTItem> _item;

        vec3d _scale_factors;
        AABB _bbox;

        [[nodiscard]] vec3d scalePoint(const vec3d &point, bool forward = true) const {
            if (forward) {
                return {point.x * _scale_factors.x, point.y * _scale_factors.y, point.z * _scale_factors.z};
            } else {
                return {point.x / _scale_factors.x, point.y / _scale_factors.y, point.z / _scale_factors.z};
            }
        }
    };

} // RN

#endif //RAYON_CPP_CMAKE_RTSCALE_H