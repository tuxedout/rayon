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
 *      RTLine.h this shit is not working yet
 *
 */

#ifndef RAYON_CPP_CMAKE_RTLINE_H
#define RAYON_CPP_CMAKE_RTLINE_H

#include "../RTDrawable.h"

namespace RN {

    class RTLine: public RTDrawable {
    public:
        RTLine(const RTLine &other)
                : RTDrawable(other),
                  _start(other._start),
                  _end(other._end),
                  _thickness(other._thickness) {
            initialize();
        }

        RTLine(const vec3d &start, const vec3d &end, double thickness, const std::shared_ptr<RN::Materials::Material> &mat)
                : RTDrawable(mat), _start(start), _end(end), _thickness(thickness) {
            initialize();
        }

//        bool hit(const ray3 &ray, double dmin, double dmax, RTHitInfo &hit_info) const override {
//
//
//            hit_info.distance = t;
//            hit_info.point = intersection;
//            hit_info.mat = material;
//            hit_info.uv = vec2d(u, 1);  // UV mapping for line
//
//            return true;
//        };

        bool hit(const ray3 &ray, double dmin, double dmax, RTHitInfo &hit_info) const override {
            vec3d line_direction = _end - _start;
            vec3d ray_to_start = _start - ray.origin;
            vec3d cross_prod = vec3d::cross(ray.direction, line_direction);

            double denom = cross_prod.length_sqr();
            if (denom < RN::EPSILON_D)
                return false;

            double t = vec3d::dot(cross_prod, ray_to_start) / denom;
            if (t < dmin || t > dmax)
                return false;

            vec3d intersection = ray.at(t);
            vec3d intersection_relative = intersection - _start;
            double u = vec3d::dot(intersection_relative, line_direction) / line_direction.length_sqr();

            if (u < 0.0 || u > 1.0)
                return false;

            vec3d closest_point = _start + line_direction * u;
            vec3d to_intersection = intersection - closest_point;

            if (to_intersection.length_sqr() > (_thickness * _thickness))
                return false;

            hit_info.distance = t;
            hit_info.point = intersection;
            hit_info.mat = material;
            hit_info.uv = vec2d(u, 1);  // UV mapping for line

            return true;
        };

        double pdfValue(const vec3d &origin, const vec3d &direction) const override {
            RTHitInfo hit_info;

            if (!this->hit(ray3(origin, direction), 0.0001, std::numeric_limits<double>::infinity(), hit_info))
                return 0;

            auto distance_squared = hit_info.distance * hit_info.distance * direction.length_sqr();
            auto cosine = fabs(vec3d::dot(direction, hit_info.normal) / direction.length());

            return distance_squared / cosine;
        }

        vec3d random(const vec3d &origin) const override {
            vec3d random_point_on_line = _start + ((_end - _start) * RN::random());
            vec3d random_offset = vec3d::random_in_unit_disk() * _thickness;
            return random_point_on_line + random_offset - origin;
        }

        [[nodiscard]] RTItemType type() const override { return rt_line; }

        [[nodiscard]] const AABB &bbox() const override {
            return _bbox;
        };

    private:
        void initialize() {
            vec3d padding(_thickness, _thickness, _thickness);
            _bbox = AABB(_start - padding, _end + padding);
            _bbox.addPadding();

            _direction = _end - _start;
        }

        double distanceToRay(const ray3& ray) {
            vec3d AP0 = ray.origin - _start;
            double d_dot_d = ray.direction.dot(ray.direction);
            double AB_dot_AB = _direction.dot(_direction);
            double d_dot_AB = _direction.dot(_direction);
            double d_dot_AP0 = _direction.dot(AP0);
            double AB_dot_AP0 = _direction.dot(AP0);

            double denominator = d_dot_d * AB_dot_AB - d_dot_AB * d_dot_AB;
            if (denominator == 0.0) {
                return AP0.norm(); // Параллельные луч и отрезок
            }

            double t = (d_dot_AB * AB_dot_AP0 - AB_dot_AB * d_dot_AP0) / denominator;
            double u = (d_dot_d * AB_dot_AP0 - d_dot_AB * d_dot_AP0) / denominator;

            // Ограничиваем u значениями от 0 до 1, так как точка Q должна быть на отрезке AB
            if (u < 0.0) u = 0.0;
            if (u > 1.0) u = 1.0;

            vec3d P = ray.origin + ray.direction * t;
            vec3d Q = _start + _direction * u;
            return (P - Q).norm();
        }

        vec3d _start;
        vec3d _end;

        // direction from start to end
        vec3d _direction;
        double _thickness;
        AABB _bbox;
    };

} // RN

#endif //RAYON_CPP_CMAKE_RTLINE_H