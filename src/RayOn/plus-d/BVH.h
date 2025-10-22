/*
 *
 *       Created by tux on 27.03.2024.
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
 *      BVH.h - simple BVH realization
 *
 */
#ifndef RAYON_BVH_H
#define RAYON_BVH_H

#include "Containers/RTScene.h"

namespace RN {

    class BVH: public RTScene {
    public:
        BVH(): RTScene(){
        }


        explicit BVH(RTScene &scene):BVH(scene.children(), 0, scene.children().size()) {
            background = scene.background;
        }

        explicit BVH(const std::vector<std::shared_ptr<RTItem>>& items):BVH(items, 0, items.size()) {
            background = {0,0,0};
        }

        BVH(const std::vector<std::shared_ptr<RTItem>>& objects, size_t start, size_t end);

        [[nodiscard]] RTItemType type() const override {return rt_bvh;}

        bool hit(const ray3 &ray, double dmin, double dmax, RTHitInfo &hit_info) const override {
            if (! _bbox.intersects(ray, dmin, dmax)) {
                return false;
            }

            bool hit_left = _left->hit(ray, dmin, dmax, hit_info);

            bool hit_right = _right->hit(ray, dmin, hit_left ? (hit_info.distance - RN::EPSILON_D) : dmax, hit_info);
//
            return hit_left || hit_right;
        }

        [[nodiscard]] const AABB &bbox() const override{
            return _bbox;
        }

        static bool box_compare(
                const std::shared_ptr<RTItem> &a, const std::shared_ptr<RTItem> &b, int axis_index
        ) {
            return a->bbox().min[axis_index] < b->bbox().min[axis_index];
        }

        static bool box_x_compare (const std::shared_ptr<RTItem> &a, const std::shared_ptr<RTItem> &b) {
            return box_compare(a, b, 0);
        }

        static bool box_y_compare (const std::shared_ptr<RTItem> &a, const std::shared_ptr<RTItem> &b) {
            return box_compare(a, b, 1);
        }

        static bool box_z_compare (const std::shared_ptr<RTItem> &a, const std::shared_ptr<RTItem> &b) {
            return box_compare(a, b, 2);
        }
    private:
        AABB _bbox;
        std::shared_ptr<RTItem> _left;
        std::shared_ptr<RTItem> _right;
    };

} // RN

#endif // RAYON_BVH_H
