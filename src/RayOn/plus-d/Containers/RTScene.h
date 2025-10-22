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
 *      RTScene.h
 *
 */

#ifndef RAYON_RTSCENE_H
#define RAYON_RTSCENE_H

#include <vector>
#include <memory>
#include "../RTDrawable.h"

namespace RN {

    class RTScene: public RTItem {
    public:
        RTScene(): RTItem(){
        }

        virtual void add(const std::shared_ptr<RTItem> &d) {
            _children.push_back(d);
            _bbox.include(d->bbox());
        }

        bool hit(const ray3 &ray, double dmin, double dmax, RTHitInfo &hit_info) const override{
            RTHitInfo tmp;

            bool lcl_hit = false;
            auto distance = dmax;

            for (const auto& object : _children) {
                if (object->hit(ray, dmin, distance, tmp)) {
                    lcl_hit = true;
                    distance = tmp.distance;
                    hit_info = tmp;
                }
            }

            return lcl_hit;
        };

        [[nodiscard]] bool intersects(const AABB &box) const {
            for (const auto& object : _children) {
                if (box.intersects(object->bbox())) {
                    return true;
                }
            }

            return false;
        }

        const AABB &bbox() const override{
            return _bbox;
        }

        [[nodiscard]] RTItemType type() const override {return rt_scene;}

        [[nodiscard]] const std::vector<std::shared_ptr<RTItem>> &children() const {
            return _children;
        }

        double pdfValue(const vec3d &origin, const vec3d &direction) const override {
            auto weight = 1.0 / _children.size();
            auto sum = 0.0;

            for (const auto& object : _children)
                sum += weight * object->pdfValue(origin, direction);

            return sum;
        }

        vec3d random(const vec3d &origin) const override {
            auto int_size = int(_children.size());
            return _children[random_int(0, int_size-1)]->random(origin);
        }

        vec3d background = {};

        std::vector<std::shared_ptr<RTItem>> children() {
            return _children;
        }
    private:
        AABB _bbox = {};
        std::vector<std::shared_ptr<RTItem>> _children;
    };

} // RN

#endif //RAYON_RTSCENE_H
