/*
 *
 *       Created by tux on 23.03.2024.
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
 *      RTSphere.h
 *
 */

#ifndef RAYON_RTSPHERE_H
#define RAYON_RTSPHERE_H

#include <memory>
#include "../RTDrawable.h"
#include "../../RayOn.h"
#include "../../CMS/ONB.h"

namespace RN {
    class RTSphere: public RTDrawable {
    public:
        explicit RTSphere(const vec3d c, const double r, const std::shared_ptr<RN::Materials::Material> &mat): RTDrawable(mat), _center(c), _radius(r){
            _bbox.min = _center - _radius;
            _bbox.max = _center + _radius;
        }

        RTSphere(const RTSphere &other): RTDrawable(other){
            _radius = other._radius;
            _center = other._center;

            _bbox.min = _center - _radius;
            _bbox.max = _center + _radius;
        }

        bool hit(const ray3 &ray, double dmin, double dmax, RTHitInfo &hit_info) const override {
            vec3 oc = ray.origin - _center;
            auto a = ray.direction.length_sqr();
            auto half_b = vec3d::dot(oc, ray.direction);
            auto c = oc.length_sqr() - _radius * _radius;

            auto discriminant = half_b * half_b - a * c;

            if (discriminant < 0) {
                return false;
            }

            auto sqrd = sqrt(discriminant);

            auto root = (-half_b - sqrd) / a;
            if (root <= dmin || dmax <= root) {
                root = (-half_b + sqrd) / a;
                if (root <= dmin || dmax <= root)
                    return false;
            }

            //hit_info.item =  this;
            hit_info.distance = root;
            hit_info.point = ray.at(root);
            hit_info.normal = (hit_info.point - _center) / _radius;
            hit_info.set_face_normal(ray, hit_info.normal);
            //hit_info.item = shared_from_this();
            hit_info.mat = material;

            auto theta = acos(hit_info.normal.y);
            auto phi = atan2(hit_info.normal.z, hit_info.normal.x) + PI_D;

            hit_info.uv = vec2d (phi / (2*PI_D), theta / PI_D);

            return true;
        }

//        bool hit(const ray& r, interval ray_t) const {
//            for (int a = 0; a < 3; a++) {
//                auto invD = 1 / r.direction()[a];
//                auto orig = r.origin()[a];
//
//                auto t0 = (axis(a).min - orig) * invD;
//                auto t1 = (axis(a).max - orig) * invD;
//
//                if (invD < 0)
//                    std::swap(t0, t1);
//
//                if (t0 > ray_t.min) ray_t.min = t0;
//                if (t1 < ray_t.max) ray_t.max = t1;
//
//                if (ray_t.max <= ray_t.min)
//                    return false;
//            }
//            return true;
//        }

        ~RTSphere() = default;

        [[nodiscard]] RTItemType type() const override {return rt_sphere;}

        [[nodiscard]] vec3d center() const {
            return _center;
        }

        [[nodiscard]] double radius() const {
            return _radius;
        }

        [[nodiscard]] const AABB &bbox() const override {
            return _bbox;
        }

        double pdfValue(const vec3d &origin, const vec3d &direction) const override {
            // This method only works for stationary spheres.
            RTHitInfo rec;
            if (!this->hit(ray3(origin, direction), 0.0001, std::numeric_limits<double>::infinity(), rec))
                return 0;

            auto cos_theta_max = sqrt(1 - _radius*_radius/(_center - origin).length_sqr());
            auto solid_angle = 2*RN::PI_D*(1-cos_theta_max);

            return  1 / solid_angle;
        }

        vec3d random(const vec3d& origin) const override {
            vec3d direction = _center - origin;
            auto distance_squared = direction.length_sqr();
            ONB uvw(direction);
            return uvw.local(randomToSphere( distance_squared));
        }
    private:
        AABB _bbox;
        double _radius;
        vec3d _center;

        vec3d randomToSphere(double distance_squared) const {
            auto r1 = RN::random();
            auto r2 = RN::random();
            auto z = 1 + r2*(sqrt(1-_radius*_radius/distance_squared) - 1);

            auto phi = 2*RN::PI_D*r1;
            auto x = cos(phi)*sqrt(1-z*z);
            auto y = sin(phi)*sqrt(1-z*z);

            return {x, y, z};
        }
    };
}

#endif //RAYON_RTSPHERE_H
