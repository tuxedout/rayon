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
 *      RTTranslate.h - translates RTItem in world space
 *
 */
#ifndef RAYON_CPP_CMAKE_TRRTTRANSLATE_H
#define RAYON_CPP_CMAKE_TRRTTRANSLATE_H

#include "../RTDrawable.h"

namespace RN::Transforms {

    class RTTranslate: public RTItem  {
    public:
        RTTranslate(std::shared_ptr<RTItem> obj, const vec3d d): RTItem() {
            _item = obj;
            _bbox = _item->bbox() + d;
            _delta = d;
        }

        bool hit(const ray3 &ray, double dmin, double dmax, RTHitInfo &hit_info) const override {
            // Change the ray from world space to object space
            // Determine whether an intersection exists in object space (and if so, where)
            if (!_item->hit(ray - _delta, dmin, dmax, hit_info))
                return false;

            // Change the intersection point from object space to world space
            auto p = hit_info.point + _delta;

            hit_info.point = p;

            return true;
        }

        [[nodiscard]] const AABB &bbox() const override {
            return _bbox;
        }

        [[nodiscard]] RTItemType type() const override {
            return rt_translate;
        }

        [[nodiscard]] double pdfValue(const vec3d &origin, const vec3d &direction) const override {
            return _item->pdfValue(origin - _delta, direction);
        }

        [[nodiscard]] vec3d random(const vec3d &origin) const override {
            return _item->random(origin - _delta);
        }
    private:
        std::shared_ptr<RTItem> _item;
        vec3d _delta;
        AABB _bbox;
    };

} // RN

#endif //RAYON_CPP_CMAKE_TRRTTRANSLATE_H
