/*
 *
 *       Created by tux on 26.03.2024.
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
 *      AABB.h - axis-aligned bounding box
 *
 */

#ifndef RAYON_AABB_H
#define RAYON_AABB_H

#include "../Vec3.h"
#include "../../Misc/Interval.h"
#include <algorithm>

namespace RN {

    class AABB {
    public:
        vec3d min = {std::numeric_limits<double>::infinity(),std::numeric_limits<double>::infinity(),std::numeric_limits<double>::infinity()};
        vec3d max = {-std::numeric_limits<double>::infinity(),-std::numeric_limits<double>::infinity(),-std::numeric_limits<double>::infinity()};

        AABB() = default;

        //Interval<double> x, y, z = {};
        AABB(const vec3d &center, const double &width, const double &height, const double &deep) {
            min = center - vec3(width, height, deep)/2.0;
            max = center + vec3(width, height, deep)/2.0;

            correctMinMax();
            addPadding();
        }

        AABB(const vec3d &min, const vec3d &max) : min(min), max(max) {
            correctMinMax();
            addPadding();
        };

        AABB(const AABB &a, const AABB &b) {
            min.x = std::min(a.min.x, b.min.x);
            min.y = std::min(a.min.y, b.min.y);
            min.z = std::min(a.min.z, b.min.z);

            max.x = std::max(a.max.x, b.max.x);
            max.y = std::max(a.max.y, b.max.y);
            max.z = std::max(a.max.z, b.max.z);


            correctMinMax();
            addPadding();
        }

        AABB(const AABB &other) : min(other.min), max(other.max) {

            correctMinMax();
            addPadding();
        }

        vec3d getSize() const {
            return max - min;
        }

        void correctMinMax() {
            if (min.x > max.x) std::swap(min.x, max.x);
            if (min.y > max.y) std::swap(min.y, max.y);
            if (min.z > max.z) std::swap(min.z, max.z);
        }

        void addPadding() {
            const double padding = 0.0001;

            min = min.sub(vec3d(padding, padding, padding));
            max = max.add(vec3d(padding, padding, padding));
        }

        void include(const AABB &a) {
            min.x = std::min(a.min.x, min.x);
            min.y = std::min(a.min.y, min.y);
            min.z = std::min(a.min.z, min.z);

            max.x = std::max(a.max.x, max.x);
            max.y = std::max(a.max.y, max.y);
            max.z = std::max(a.max.z, max.z);
        }

        [[nodiscard]] bool intersects(const AABB &other) const {
            return (min.x <= other.max.x && max.x >= other.min.x) &&
                   (min.y <= other.max.y && max.y >= other.min.y) &&
                   (min.z <= other.max.z && max.z >= other.min.z);
        }

        [[nodiscard]] bool contains(const vec3d &point) const {
            return (point.x >= min.x && point.x <= max.x) &&
                   (point.y >= min.y && point.y <= max.y) &&
                   (point.z >= min.z && point.z <= max.z);
        }

        // Расширение AABB до включения точки
        void expandToInclude(const vec3d &point) {
            min.x = std::min(min.x, point.x);
            min.y = std::min(min.y, point.y);
            min.z = std::min(min.z, point.z);

            max.x = std::max(max.x, point.x);
            max.y = std::max(max.y, point.y);
            max.z = std::max(max.z, point.z);
        }

        [[nodiscard]] vec3d getCenter() const {
            return { (min.x + max.x) / 2, (min.y + max.y) / 2, (min.z + max.z) / 2 };
        }

        bool intersects(const ray3 &ray, double &d_min, double &d_max) const {
//            return true;
            //vec3 tmp_origin = ray.origin;
            //vec3 tmp_direction = ray.direction;

            const double eps = 0.000001;

            // x axis
            if (std::abs(ray.direction.x) < eps) {
                // Луч параллелен одной из осей
                if (ray.origin.x < min.x || ray.origin.x > max.x) {
                    return false; // Луч не пересекает AABB
                }
            } else {
                // Находим параметры t, при которых луч пересекает плоскости AABB
                double ood = 1.0f / ray.direction.x;
                double t1 = (min.x - ray.origin.x) * ood;
                double t2 = (max.x - ray.origin.x) * ood;
                if (t1 > t2) std::swap(t1, t2);
                d_min = std::max(d_min, t1);
                d_max = std::min(d_max, t2);
                if (d_min > d_max) return false;
            }

            // y axis
            if (std::abs(ray.direction.y) < eps) {
                // Луч параллелен одной из осей
                if (ray.origin.y < min.y || ray.origin.y > max.y) {
                    return false; // Луч не пересекает AABB
                }
            } else {
                // Находим параметры t, при которых луч пересекает плоскости AABB
                double ood = 1.0f / ray.direction.y;
                double t1 = (min.y - ray.origin.y) * ood;
                double t2 = (max.y - ray.origin.y) * ood;
                if (t1 > t2) std::swap(t1, t2);
                d_min = std::max(d_min, t1);
                d_max = std::min(d_max, t2);
                if (d_min > d_max) return false;
            }

            // z axis
            if (std::abs(ray.direction.z) < eps) {
                // Луч параллелен одной из осей
                if (ray.origin.z < min.z || ray.origin.z > max.z) {
                    return false; // Луч не пересекает AABB
                }
            } else {
                // Находим параметры t, при которых луч пересекает плоскости AABB
                double ood = 1.0f / ray.direction.z;
                double t1 = (min.z - ray.origin.z) * ood;
                double t2 = (max.z - ray.origin.z) * ood;
                if (t1 > t2) std::swap(t1, t2);
                d_min = std::max(d_min, t1);
                d_max = std::min(d_max, t2);
                if (d_min > d_max) return false;
            }

            return true;
        }

        AABB operator+(const vec3d v) const {
            return {min + v, max + v};
        }

        vec3d operator[](const int i) const {
            switch (i) {
                case 0: {
                    return min;
                }
                case 1: {
                    return {max.x, min.y, min.z};
                }
                case 2: {
                    return {min.x, min.y, max.z};
                }
                case 3: {
                    return {max.x, min.y, max.z};
                }
                case 4: {
                    return max;
                }
                case 5: {
                    return {min.x, max.y, min.z};
                }
                case 6: {
                    return {min.x, max.y, max.z};
                }
                case 7: {
                    return {max.x, min.y, max.z};
                }
                default:{
                    std::cerr << "AABB[] out of index: " << i << "\n";
                    return {0};
                }
            }
        }

//        bool intersects(const ray3 &ray, double &d_min, double &d_max) const {
//            for (int i = 0; i < 3; ++i) {
//                if (std::abs(ray.direction[i]) < std::numeric_limits<float>::epsilon()) {
//                    // Луч параллелен одной из осей
//                    if (ray.origin[i] < min[i] || ray.origin[i] > max[i]) {
//                        return false; // Луч не пересекает AABB
//                    }
//                } else {
//                    // Находим параметры t, при которых луч пересекает плоскости AABB
//                    double ood = 1.0f / ray.direction[i];
//                    double t1 = (min[i] - ray.origin[i]) * ood;
//                    double t2 = (max[i] - ray.origin[i]) * ood;
//                    if (t1 > t2) std::swap(t1, t2);
//                    d_min = std::max(d_min, t1);
//                    d_max = std::min(d_max, t2);
//                    if (d_min > d_max) return false;
//                }
//            }
//            return true;
//        }
    };
} // RN

#endif //RAYON_AABB_H
