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
 *      RTRotate.h - rotates RTItem in world space around axis specified as vec3d
 *
 */
#ifndef RAYON_CPP_CMAKE_RTROTATE_H
#define RAYON_CPP_CMAKE_RTROTATE_H

#include "../RTDrawable.h"

namespace RN::Transforms {

    class RTRotate: public RTItem  {
    public:
        RTRotate(std::shared_ptr<RTItem> obj, const vec3d& axis, const double radians): RTItem() {
            _item = obj;
            _angle = radians;
            _axis = axis;
            _axis.normalize();

            _sin_theta = sin(_angle);
            _cos_theta = cos(_angle);

            _bbox = _item->bbox();

            vec3d min( std::numeric_limits<double>::infinity(),  std::numeric_limits<double>::infinity(),  std::numeric_limits<double>::infinity());
            vec3d max(-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity());

            for (int i = 0; i < 8; i++) {
                vec3d point = _bbox[i];
                vec3d rotated_point = rotatePoint(point);

                min = vec3d::min(min, rotated_point);
                max = vec3d::max(max, rotated_point);
            }

            _bbox = AABB(min, max);
        }

        bool hit(const ray3 &ray, double dmin, double dmax, RTHitInfo &hit_info) const override {
            auto origin = rotatePoint(ray.origin);
            auto direction = rotatePoint(ray.direction);

            ray3 rotated_r(origin, direction);

            if (!_item->hit(rotated_r, dmin, dmax, hit_info))
                return false;

            hit_info.point = rotatePoint(hit_info.point, false);
            hit_info.normal = rotatePoint(hit_info.normal, false);

            return true;
        }

        const AABB &bbox() const override {
            return _bbox;
        }

        RTItemType type() const override {
            return rt_rotate;
        }

        double pdfValue(const vec3d &origin, const vec3d &direction) const override {
            auto lcl_origin = rotatePoint(origin);
            auto lcl_direction = rotatePoint(direction);

            return _item->pdfValue(lcl_origin, lcl_direction);
        }

        vec3d random(const vec3d &origin) const override {
            auto lcl_origin = rotatePoint(origin);

            return _item->random(origin);
        }
    private:
        std::shared_ptr<RTItem> _item;

        vec3d _axis; // axis to rotate around 8=>
        double _angle; // angle in radians
        double _sin_theta;
        double _cos_theta;
        AABB _bbox;

        [[nodiscard]] vec3d rotatePoint(const vec3d &point, bool forward = true) const {
            double sin_theta = forward ? _sin_theta : -_sin_theta;
            double cos_theta = _cos_theta;

            double u = _axis.x;
            double v = _axis.y;
            double w = _axis.z;

            double x = point.x;
            double y = point.y;
            double z = point.z;

            double newx = u * (u * x + v * y + w * z) * (1 - cos_theta) + x * cos_theta + (-w * y + v * z) * sin_theta;
            double newy = v * (u * x + v * y + w * z) * (1 - cos_theta) + y * cos_theta + (w * x - u * z) * sin_theta;
            double newz = w * (u * x + v * y + w * z) * (1 - cos_theta) + z * cos_theta + (-v * x + u * y) * sin_theta;

            return {newx, newy, newz};
        }
    };

} // RN

#endif //RAYON_CPP_CMAKE_RTROTATE_H