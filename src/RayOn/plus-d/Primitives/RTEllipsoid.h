/*
 *
 *       Created by tux on 17.04.2024.
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
 *      RTEllipsoid.h
 *
 */

#ifndef RAYON_CPP_CMAKE_RTELLIPSOID_H
#define RAYON_CPP_CMAKE_RTELLIPSOID_H

#include <memory>
#include "../RTDrawable.h"
#include "../../RayOn.h"

namespace RN {
    class RTEllipsoid : public RTDrawable {
    public:
        RTEllipsoid(const vec3d& center, const vec3d& radii, const std::shared_ptr<RN::Materials::Material> &mat)
                : RTDrawable(mat), _center(center), _radii(radii) {
            _bbox.min = _center - _radii;
            _bbox.max = _center + _radii;
        }

        RTEllipsoid(const RTEllipsoid& other) : RTDrawable(other) {
            _center = other._center;
            _radii = other._radii;
            _bbox = other._bbox;
        }

        bool hit(const ray3& ray, double dmin, double dmax, RTHitInfo& hit_info) const override {
            vec3d oc = ray.origin - _center;
            vec3d radii_squared = _radii * _radii;

            auto a = ray.direction.dot(ray.direction / radii_squared);
            auto half_b = vec3d::dot(oc / radii_squared, ray.direction);
            auto c = vec3d::dot(oc / radii_squared, oc) - 1;

            auto discriminant = half_b * half_b - a * c;
            if (discriminant < 0) return false;

            auto sqrd = sqrt(discriminant);
            auto root = (-half_b - sqrd) / a;
            if (root <= dmin || dmax <= root) {
                root = (-half_b + sqrd) / a;
                if (root <= dmin || dmax <= root)
                    return false;
            }

            hit_info.distance = root;
            hit_info.point = ray.at(root);
            vec3d normal = (hit_info.point - _center) / radii_squared;
            normal.normalize();
            hit_info.normal = normal;
            hit_info.set_face_normal(ray, hit_info.normal);
            hit_info.mat = material;

            auto theta = acos(hit_info.normal.y / hit_info.normal.length());
            auto phi = atan2(hit_info.normal.z, hit_info.normal.x) + PI_D;

            hit_info.uv = vec2d(phi / (2 * PI_D), theta / PI_D);

            return true;
        }

        ~RTEllipsoid() = default;

        [[nodiscard]] RTItemType type() const override { return rt_ellipsoid; }

        [[nodiscard]] vec3d center() const { return _center; }
        [[nodiscard]] vec3d radii() const { return _radii; }
        [[nodiscard]] const AABB& bbox() const override { return _bbox; }

    private:
        AABB _bbox;
        vec3d _center;
        vec3d _radii;
    };
}

#endif //RAYON_CPP_CMAKE_RTELLIPSOID_H