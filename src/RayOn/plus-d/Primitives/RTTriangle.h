/*
 *
 *       Created by tux on 18.04.2024.
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
 *      RTTriangle.h
 *
 */

#ifndef RAYON_CPP_CMAKE_RTTRIANGLE_H
#define RAYON_CPP_CMAKE_RTTRIANGLE_H

#include "../RTDrawable.h"

namespace RN {

    class RTTriangle: public RTDrawable {
    public:
        // vertexes
        vec3d v0, v1, v2;

        // vertex normals
        vec3d normal0, normal1, normal2;

        // face normal
        vec3d normal;

        vec3d centroid;

        RTTriangle(
                const vec3d &v0,
                const vec3d &v1,
                const vec3d &v2,
                const vec3d &normal0,
                const vec3d &normal1,
                const vec3d &normal2,
                const std::shared_ptr<RN::Materials::Material> &mat):
                RTDrawable(mat),
                v0(v0),
                v1(v1),
                v2(v2),
                normal((normal0 + normal1 + normal2)/2),
                normal0(normal0),
                normal1(normal1),
                normal2(normal2)
        {

            calculateBBox();
            calculateCentroid();
        }

        RTTriangle(
                const vec3d &v0,
                const vec3d &v1,
                const vec3d &v2,
                const vec3d &normal,
                const vec3d &normal0,
                const vec3d &normal1,
                const vec3d &normal2,
                const std::shared_ptr<RN::Materials::Material> &mat):
                RTDrawable(mat),
                v0(v0),
                v1(v1),
                v2(v2),
                normal(normal),
                normal0(normal0),
                normal1(normal1),
                normal2(normal2)
                {
            // init min and max coords of _bbox
            vec3d minB = v0;
            vec3d maxB = v0;

            // update with 2nd vrtx
            minB.x = std::min(minB.x, v1.x);
            minB.y = std::min(minB.y, v1.y);
            minB.z = std::min(minB.z, v1.z);

            maxB.x = std::max(maxB.x, v1.x);
            maxB.y = std::max(maxB.y, v1.y);
            maxB.z = std::max(maxB.z, v1.z);

            // update with 3rd vrtx
            minB.x = std::min(minB.x, v2.x);
            minB.y = std::min(minB.y, v2.y);
            minB.z = std::min(minB.z, v2.z);

            maxB.x = std::max(maxB.x, v2.x);
            maxB.y = std::max(maxB.y, v2.y);
            maxB.z = std::max(maxB.z, v2.z);

            // create AABB
            _bbox = AABB(minB, maxB);

            calculateCentroid();
        }

        RTTriangle(const vec3d &v0, const vec3d &v1, const vec3d &v2, const std::shared_ptr<RN::Materials::Material> &mat): RTDrawable(mat), v0(v0), v1(v1), v2(v2) {
            // init min and max coords of _bbox
            vec3d minB = v0;
            vec3d maxB = v0;

            // update with 2nd vrtx
            minB.x = std::min(minB.x, v1.x);
            minB.y = std::min(minB.y, v1.y);
            minB.z = std::min(minB.z, v1.z);

            maxB.x = std::max(maxB.x, v1.x);
            maxB.y = std::max(maxB.y, v1.y);
            maxB.z = std::max(maxB.z, v1.z);

            // update with 3rd vrtx
            minB.x = std::min(minB.x, v2.x);
            minB.y = std::min(minB.y, v2.y);
            minB.z = std::min(minB.z, v2.z);

            maxB.x = std::max(maxB.x, v2.x);
            maxB.y = std::max(maxB.y, v2.y);
            maxB.z = std::max(maxB.z, v2.z);

            // create AABB
            _bbox = AABB(minB, maxB);

            calculateNormal();
            calculateCentroid();
        }

        RTTriangle(const RTTriangle &other):
            RTDrawable(other),
            v0(other.v0),
            v1(other.v1),
            v2(other.v2),
            normal(other.normal),
            normal0(other.normal0),
            normal1(other.normal1),
            normal2(other.normal2),
            centroid(other.centroid)
            {

            // init min and max coords of _bbox
            vec3d minB = v0;
            vec3d maxB = v0;

            // update with 2nd vrtx
            minB.x = std::min(minB.x, v1.x);
            minB.y = std::min(minB.y, v1.y);
            minB.z = std::min(minB.z, v1.z);

            maxB.x = std::max(maxB.x, v1.x);
            maxB.y = std::max(maxB.y, v1.y);
            maxB.z = std::max(maxB.z, v1.z);

            // update with 3rd vrtx
            minB.x = std::min(minB.x, v2.x);
            minB.y = std::min(minB.y, v2.y);
            minB.z = std::min(minB.z, v2.z);

            maxB.x = std::max(maxB.x, v2.x);
            maxB.y = std::max(maxB.y, v2.y);
            maxB.z = std::max(maxB.z, v2.z);

            // create AABB
            _bbox = AABB(minB, maxB);
        }

        bool hit(const ray3 &ray, double dmin, double dmax, RTHitInfo &hit_info) const override {
            vec3 v1v0 = v1 - v0;
            vec3 v2v0 = v2 - v0;
            vec3 rov0 = ray.origin - v0;
            vec3 n = vec3d::cross(v1v0, v2v0);

            vec3 q = vec3d::cross(rov0, ray.direction);
            double d = 1.0 / vec3d::dot(ray.direction, n);

            double u = d * vec3d::dot(-q, v2v0);
            double v = d * vec3d::dot(q, v1v0);
            double t = d * vec3d::dot(-n, rov0);

            // Check if the intersection is within the bounds of the triangle
            if (u < 0 || u > 1 || v < 0 || (u + v) > 1) {
                return false;
            }

            // Check if the intersection is within the acceptable range
            if (t < dmin || t > dmax) {
                return false;
            }

            // Fill hit record
            hit_info.distance = t;
            hit_info.point = ray.at(t);

            // Calculate interpolated normal using barycentric coordinates
            double w = 1.0 - u - v;
            vec3 interpolated_normal = (normal0 * w) + (normal1 * u) + (normal2 * v);
            interpolated_normal.normalize();

            hit_info.normal = interpolated_normal;

            hit_info.set_face_normal(ray, hit_info.normal);
            hit_info.mat = material;

            // Calculate texture coordinates (u, v) if needed
            hit_info.uv = vec2(u, v);

            return true;
        }

//        bool hit(const ray3 &ray, double dmin, double dmax, RTHitInfo &hit_info) const override {
//            vec3 v1v0 = v1 - v0;
//            vec3 v2v0 = v2 - v0;
//            vec3 rov0 = ray.origin - v0;
//            vec3 n = vec3d::cross(v1v0, v2v0);
//
//            vec3 q = vec3d::cross(rov0, ray.direction);
//            double d = 1.0 / vec3d::dot(ray.direction, n);
//
//            double u = d * vec3d::dot(-q, v2v0);
//            double v = d * vec3d::dot(q, v1v0);
//            double t = d * vec3d::dot(-n, rov0);
//
//            // Check if the intersection is within the bounds of the triangle
//            if (u < 0 || u > 1 || v < 0 || (u + v) > 1) {
//                return false;
//            }
//
//            // Check if the intersection is within the acceptable range
//            if (t < dmin || t > dmax) {
//                return false;
//            }
//
//            // Fill hit record
//            hit_info.distance = t;
//            hit_info.point = ray.at(t);
//
//            hit_info.normal = n;
//
//            hit_info.normal.normalize();
//            hit_info.set_face_normal(ray, hit_info.normal);
//            hit_info.mat = material;
//
//            // Calculate texture coordinates (u, v) if needed
//            hit_info.uv = vec2(u, v);
//
//            return true;
//        }

        // try to calculate normal
        // warning: will overwrite vertex normal
        void calculateNormal() {
            vec3 v1v0 = v1 - v0;
            vec3 v2v0 = v2 - v0;

            normal = vec3d::cross(v1v0, v2v0);
            normal.normalize();

            normal0 = normal;
            normal1 = normal0;
            normal2 = normal0;
        }

        void calculateCentroid() {
            centroid = (v0 + v1 + v2) / 3.0;
        }

        void calculateBBox() {
            // init min and max coords of _bbox
            vec3d minB = v0;
            vec3d maxB = v0;

            // update with 2nd vrtx
            minB.x = std::min(minB.x, v1.x);
            minB.y = std::min(minB.y, v1.y);
            minB.z = std::min(minB.z, v1.z);

            maxB.x = std::max(maxB.x, v1.x);
            maxB.y = std::max(maxB.y, v1.y);
            maxB.z = std::max(maxB.z, v1.z);

            // update with 3rd vrtx
            minB.x = std::min(minB.x, v2.x);
            minB.y = std::min(minB.y, v2.y);
            minB.z = std::min(minB.z, v2.z);

            maxB.x = std::max(maxB.x, v2.x);
            maxB.y = std::max(maxB.y, v2.y);
            maxB.z = std::max(maxB.z, v2.z);

            // create AABB
            _bbox = AABB(minB, maxB);
        }

        [[nodiscard]] const AABB &bbox() const override {
            return _bbox;
        }
    private:
        AABB _bbox;
    };

} // RN

#endif //RAYON_CPP_CMAKE_RTTRIANGLE_H
