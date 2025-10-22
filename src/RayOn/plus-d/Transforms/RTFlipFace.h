/*
 *
 *       Created by tux on 03.06.2024.
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
 *      RTFlipFace.h
 *
 */

#ifndef RAYON_CPP_CMAKE_RTFLIPFACE_H
#define RAYON_CPP_CMAKE_RTFLIPFACE_H

#include "../RTDrawable.h"

namespace RN {
    namespace Transforms {

        class RTFlipFace: public RTItem {
        public:
            explicit RTFlipFace(const std::shared_ptr<RTItem> &item): _item(item) {

            }

            bool hit(const ray3 &ray, double dmin, double dmax, RTHitInfo &hit_info) const override {

                bool was_hit = _item->hit(ray, dmin, dmax, hit_info);

                if (was_hit){hit_info.front_face = ! hit_info.front_face; }

                return was_hit;
            }

            const AABB &bbox() const override {
                return _item->bbox();
            }

            RTItemType type() const override {
                return rt_flipface;
            }

            double pdfValue(const vec3d &origin, const vec3d &direction) const override {
                return _item->pdfValue(origin, direction);
            }

            vec3d random(const vec3d &origin) const override {
                return _item->random(origin);
            }

        private:
            std::shared_ptr<RTItem> _item;
        };

    } // Transforms
} // RN

#endif //RAYON_CPP_CMAKE_RTFLIPFACE_H
