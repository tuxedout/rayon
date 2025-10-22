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
 *      RTPolygon.h
 *
 */

#ifndef RAYON_CPP_CMAKE_RTPOLYGON_H
#define RAYON_CPP_CMAKE_RTPOLYGON_H

#include "../RTDrawable.h"
#include <vector>

namespace RN {

    class RTPolygon: public RTDrawable {
    public:
        // vertices
        std::vector<vec3d> vertices;

        // vertex normals
        std::vector<vec3d> vertex_normals;

        // face normal
        vec3d normal;

        vec3d centroid;

        RTPolygon(
                const std::vector<vec3d>& vertices,
                const std::vector<vec3d>& vertex_normals,
                const std::shared_ptr<RN::Materials::Material>& mat):
                RTDrawable(mat),
                vertices(vertices),
                vertex_normals(vertex_normals)
        {
            calculateNormal();
            calculateBBox();
            calculateCentroid();
        }

        bool hit(const ray3 &ray, double dmin, double dmax, RTHitInfo &hit_info) const override {
            if (vertices.size() < 3) return false;

            vec3 v1v0 = vertices[1] - vertices[0];
            vec3 v2v0 = vertices[2] - vertices[0];
            vec3 rov0 = ray.origin - vertices[0];
            vec3 n = vec3d::cross(v1v0, v2v0);

            vec3 q = vec3d::cross(rov0, ray.direction);
            double d = 1.0 / vec3d::dot(ray.direction, n);

            double u = d * vec3d::dot(-q, v2v0);
            double v = d * vec3d::dot(q, v1v0);
            double t = d * vec3d::dot(-n, rov0);

            if (u < 0 || v < 0 || (u + v) > 1) {
                return false;
            }

            if (t < dmin || t > dmax) {
                return false;
            }

            hit_info.distance = t;
            hit_info.point = ray.at(t);

            vec3 interpolated_normal = interpolateNormal(u, v);
            interpolated_normal.normalize();

            hit_info.normal = interpolated_normal;
            hit_info.set_face_normal(ray, hit_info.normal);
            hit_info.mat = material;

            hit_info.uv = vec2(u, v);

            return true;
        }

        void calculateNormal() {
            if (vertices.size() < 3) return;

            vec3 v1v0 = vertices[1] - vertices[0];
            vec3 v2v0 = vertices[2] - vertices[0];

            normal = vec3d::cross(v1v0, v2v0);
            normal.normalize();

            vertex_normals.assign(vertices.size(), normal);
        }

        void calculateCentroid() {
            vec3d sum(0, 0, 0);
            for (const auto& vertex : vertices) {
                sum = sum + vertex;
            }
            centroid = sum / static_cast<double>(vertices.size());
        }

        void calculateBBox() {
            if (vertices.empty()) return;

            vec3d minB = vertices[0];
            vec3d maxB = vertices[0];

            for (const auto& vertex : vertices) {
                minB.x = std::min(minB.x, vertex.x);
                minB.y = std::min(minB.y, vertex.y);
                minB.z = std::min(minB.z, vertex.z);

                maxB.x = std::max(maxB.x, vertex.x);
                maxB.y = std::max(maxB.y, vertex.y);
                maxB.z = std::max(maxB.z, vertex.z);
            }

            _bbox = AABB(minB, maxB);
        }

        [[nodiscard]] const AABB &bbox() const override {
            return _bbox;
        }

    private:
        AABB _bbox;

        vec3d interpolateNormal(double u, double v) const {
            double w = 1.0 - u - v;
            return vertex_normals[0] * w + vertex_normals[1] * u + vertex_normals[2] * v;
        }
    };

} // RN

#endif //RAYON_CPP_CMAKE_RTPOLYGON_H
