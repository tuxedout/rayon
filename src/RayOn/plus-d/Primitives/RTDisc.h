/*
 *
 *       Created by tux on 07.04.2024.
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
 *      RTDisc.h - simple disc
 *
 */

#ifndef RAYON_CPP_CMAKE_RTDISC_H
#define RAYON_CPP_CMAKE_RTDISC_H

#include "../RTDrawable.h"
#include "../../RayOn.h"

namespace RN {

    class RTDisc: public RTDrawable{
    public:
        RTDisc(std::shared_ptr<RN::Materials::Material> mat): RTDrawable(mat) {
            updateBBox();
        };

        RTDisc(const vec3 &c, const vec3 &n, const double radius, std::shared_ptr<RN::Materials::Material> mat): RTDrawable(mat),
            _center(c), _normal(n), _radius(radius), _bbox() {
                updateBBox();
        };

        bool hit(const ray3 &ray, double dmin, double dmax, RTHitInfo &hit_info) const override {
            vec3 o = ray.origin - _center;
            double denom = vec3::dot(ray.direction, _normal);

            // Если луч параллелен плоскости диска, нет пересечения.
            if (std::abs(denom) < 1e-6) return false;

            double t = -vec3::dot(o, _normal) / denom;
            if (t < dmin || t > dmax) return false; // Проверка находится ли 't' в допустимых пределах.

            vec3 p = ray.origin + ray.direction * t; // Точка пересечения на диске.
            vec3 q = p - _center; // Вектор от центра диска к точке пересечения.

            // Проверка, лежит ли точка пересечения внутри радиуса диска.
            if (vec3::dot(q, q) > _radius * _radius) return false;

            // Расчет текстурных координат (пример для диска, лежащего в плоскости XY).
            double u = 0.5 + (atan2(q.z, q.x) / (2 * RN::PI_D));
            double v = 0.5 - (asin(q.y / _radius) / RN::PI_D);

            hit_info.distance = t;
            hit_info.point = p;
            hit_info.normal = (denom > 0.0) ? -_normal : _normal; // Определение внешней или внутренней стороны.
            hit_info.uv = vec2(u, v);

            return true;
        }

        [[nodiscard]] RTItemType type() const  override { return rt_disc; }

    private:
        void updateBBox(){
            _bbox.min = _center;
            _bbox.max = _center;

            // Для каждой оси проверяем, как нормаль влияет на расширение границ.
            // Если нормаль по данной оси не 0, то нужно расширить границы по этой оси на радиус.
            if (_normal.x != 0) {
                _bbox.min.x -= _radius;
                _bbox.max.x += _radius;
            }
            if (_normal.y != 0) {
                _bbox.min.x -= _radius;
                _bbox.max.x += _radius;
            }
            if (_normal.z != 0) {
                _bbox.min.z -= _radius;
                _bbox.max.z += _radius;
            }
        }

        vec3 _normal = {0,1.0,0};
        vec3 _center = {};

        AABB _bbox = {};

        double _radius = 1.0;
    };

} // RN

#endif //RAYON_CPP_CMAKE_RTDISC_H
