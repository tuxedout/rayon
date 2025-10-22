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
 *      BVH.cpp - simple BVH realization
 *
 */
#include "BVH.h"
#include "../RayOn.h"

RN::BVH::BVH(const std::vector<std::shared_ptr<RTItem>> &scene_objects, size_t start, size_t end) {
    int axis = RN::random_int(0,2);

    auto comparator = (axis == 0) ? box_x_compare
                                  : (axis == 1) ? box_y_compare
                                                : box_z_compare;

    size_t span = end - start;

    if (span == 1) {
        _left = _right = scene_objects[start];
    } else if (span == 2) {
        if (comparator(scene_objects[start], scene_objects[start+1])) {
            _left = scene_objects[start];
            _right = scene_objects[start+1];
        } else {
            _left = scene_objects[start+1];
            _right = scene_objects[start];
        }
    } else {

        // Создаем вектор ссылок
        //std::vector<std::shared_ptr<RTItem>> objects(scene_objects.begin() + start, scene_objects.begin() + end);

        auto objects = scene_objects;

        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        auto mid = start + span/2;
        _left = make_shared<BVH>(std::vector<std::shared_ptr<RTItem>>(objects.begin() + start, objects.begin() + mid));
        _right = make_shared<BVH>(std::vector<std::shared_ptr<RTItem>>(objects.begin() + mid, objects.begin() + end));
    }

//    if (_left->bbox().min.isZero() || _left->bbox().max.isZero() || (_left->bbox().max-_left->bbox().min).isZero()) {
//        std::cerr << "_left is zero" << "\n";
//    }
//
//    if (_right->bbox().min.isZero() || _right->bbox().max.isZero() || (_right->bbox().max-_right->bbox().min).isZero()) {
//        std::cerr << "_right is zero" << "\n";
//    }

    _bbox.include(_left->bbox());
    _bbox.include(_right->bbox());
}
