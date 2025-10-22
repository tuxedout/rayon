/*
 *
 *       Created by tux on 23.04.2024.
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
 *      RTCylinder.h
 *
 */

#ifndef RAYON_RTCYLINDER_H
#define RAYON_RTCYLINDER_H

#include <memory>
#include "../RTDrawable.h"
#include "../../RayOn.h"

namespace RN {
    class RTCylinder : public RTDrawable {
    public:
        explicit RTCylinder(const vec3d c, const double r, const double h, const std::shared_ptr<RN::Materials::Material>& mat)
                : RTDrawable(mat), _center(c), _radius(r), _height(h) {
            _bbox.min = _center - vec3d(_radius, _height / 2, _radius);
            _bbox.max = _center + vec3d(_radius, _height / 2, _radius);
        }

        RTCylinder(const RTCylinder& other) : RTDrawable(other) {
            _radius = other._radius;
            _height = other._height;
            _center = other._center;

            _bbox.min = _center - vec3d(_radius, _height / 2, _radius);
            _bbox.max = _center + vec3d(_radius, _height / 2, _radius);
        }

        bool hit(const ray3& ray, double dmin, double dmax, RTHitInfo& hit_info) const override {
            vec3 oc = ray.origin - _center;
            double a = ray.direction.x * ray.direction.x + ray.direction.z * ray.direction.z;
            double half_b = oc.x * ray.direction.x + oc.z * ray.direction.z;
            double c = oc.x * oc.x + oc.z * oc.z - _radius * _radius;
            double discriminant = half_b * half_b - a * c;

            double temp_root;

            if (discriminant >= 0) {
                double sqrtd = sqrt(discriminant);

                // Находим корень, соответствующий ближайшей точке столкновения
                temp_root = (-half_b - sqrtd) / a;
                if (temp_root < dmin || temp_root > dmax) {
                    temp_root = (-half_b + sqrtd) / a;
                }

                if (temp_root >= dmin && temp_root <= dmax) {
                    // Проверяем, находится ли точка столкновения в пределах высоты цилиндра
                    vec3d point = ray.at(temp_root);
                    double y = point.y - _center.y;
                    if (y >= -_height / 2 && y <= _height / 2) {
                        hit_info.distance = temp_root;
                        hit_info.point = point;
                        hit_info.normal = vec3d((hit_info.point.x - _center.x) / _radius, 0, (hit_info.point.z - _center.z) / _radius);
                        hit_info.set_face_normal(ray, hit_info.normal);
                        hit_info.mat = material;
                        return true;
                    }
                }
            }

            // Проверка на столкновение с торцами цилиндра
                double t_min = (dmin > 0 ? dmin : 0);
                double t_max = dmax;
                for (int i = 0; i < 2; i++) {
                    double y_plane = i == 0 ? _center.y - _height / 2 : _center.y + _height / 2;
                    double t_plane = (y_plane - ray.origin.y) / ray.direction.y;
                    if (t_plane >= t_min && t_plane <= t_max) {
                        vec3d p = ray.at(t_plane);
                        double x = p.x - _center.x;
                        double z = p.z - _center.z;
                        if (x * x + z * z <= _radius * _radius) {
                            hit_info.distance = t_plane;
                            hit_info.point = p;
                            hit_info.normal = vec3d(0, i == 0 ? -1 : 1, 0);
                            hit_info.set_face_normal(ray, hit_info.normal);
                            hit_info.mat = material;
                            return true; // Если луч попал в один из торцов, возвращаем true
                        }
                    }
                }

            return false;
        }

        ~RTCylinder() = default;

        [[nodiscard]] RTItemType type() const override { return rt_cylinder; }
        [[nodiscard]] vec3d center() const { return _center; }
        [[nodiscard]] double radius() const { return _radius; }
        [[nodiscard]] double height() const { return _height; }
        [[nodiscard]] const AABB& bbox() const override { return _bbox; }

    private:
        AABB _bbox;
        double _radius;
        double _height;
        vec3d _center;
    };
}

#endif //RAYON_RTCYLINDER_H