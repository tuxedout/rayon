/*
 *
 *       Created by tux on 15.04.2024.
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
 *      RTQuad.h
 *
 */

#ifndef RAYON_CPP_CMAKE_RTQUAD_H
#define RAYON_CPP_CMAKE_RTQUAD_H

#include "../RTDrawable.h"

namespace RN {

    class RTQuad: public RTDrawable {
    public:
        RTQuad(const RTQuad &other)
            : RTDrawable(other),
              _origin(other._origin),
              _u(other._u),
              _v(other._v) {
            initialize();
        }

        RTQuad(const vec3d &o, const vec3d &u, const vec3d &v, const std::shared_ptr<RN::Materials::Material> &mat, const bool flipped = false)
            : RTDrawable(mat), _origin(o), _u(u), _v(v) {
            initialize();
        }

        bool hit(const ray3 &ray, double dmin, double dmax, RTHitInfo &hit_info) const override {
            if (!_bbox.intersects(ray, dmin, dmax)){
                return false;
            }

            double denom = vec3d::dot(_normal, ray.direction);

            if (fabs(denom) < RN::EPSILON_D)
                return false;

            auto t = (_D - vec3d::dot(_normal, ray.origin)) / denom;
            if (t < dmin || t > dmax) {
                return false;
            }

            auto intersection = ray.at(t);
            vec3d intersection_relative = intersection - _origin;
            double u = vec3d::dot(intersection_relative, _u) / _u.length_sqr();
            double v = vec3d::dot(intersection_relative, _v) / _v.length_sqr();

            if (u < 0.0 || u > 1.0 || v < 0.0 || v > 1.0) {
                return false;
            }

            hit_info.distance = t;
            hit_info.point = intersection;
            hit_info.mat = material;
            hit_info.uv = vec2d(u, v);
            hit_info.set_face_normal(ray, _normal);

            return true;
        };

        double pdfValue(const vec3d &origin, const vec3d &direction) const override {
            RTHitInfo hit_info;

            if (!this->hit(ray3(origin, direction), 0.000001, std::numeric_limits<double>::infinity(), hit_info))
                return 0;

            auto distance_squared = hit_info.distance * hit_info.distance * direction.length_sqr();
            auto cosine = fabs(vec3d::dot(direction, hit_info.normal) / direction.length());

            return distance_squared / (cosine * _area);
        }

        vec3d random(const vec3d &origin) const override {
            auto p = _origin + (_u * RN::random()) + ( _v * RN::random());
            return p - origin;
        }

        [[nodiscard]] RTItemType type() const override {return rt_quad;}

        [[nodiscard]] const AABB &bbox() const override {
            return _bbox;
        };

    private:
        void initialize() {
            auto bbox_diagonal1 = RN::AABB(_origin, _origin + _u + _v);
            auto bbox_diagonal2 = RN::AABB(_origin + _u, _origin + _v);
            _bbox = RN::AABB(bbox_diagonal1, bbox_diagonal2);

            _normal = vec3d::cross(_u, _v);
            _area = _normal.length();
            _normal.normalize();

            _D = vec3d::dot(_normal, _origin);
        }

        vec3d _origin;
        vec3d _normal;
        vec3d _u;
        vec3d _v;
        double _area;
        double _D;
        RN::AABB _bbox;
    };

} // RN

#endif //RAYON_CPP_CMAKE_RTQUAD_H

