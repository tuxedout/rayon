/*
 *
 *       Created by tux on 24.03.2024.
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
 *      RTBox.h
 *
 */

#ifndef RAYON_RTBOX_H
#define RAYON_RTBOX_H

#include "../RTDrawable.h"
#include "RTQuad.h"
#include "../Transforms/RTFlipFace.h"

namespace RN {

    class RTBox: public RTDrawable {
    public:
        RTBox(const vec3d c, const vec3d s, const std::shared_ptr<RN::Materials::Material> &mat): RTDrawable(mat), _center(c), _size(s){
            _rad = _size / 2.0;
            _min = _center - _rad;
            _max = _center + _rad;
            _bbox = AABB(_center - _rad, _center + _rad);

            RTQuad sideXZBottom(_center - _rad, {_size.x, 0, 0}, {0, 0, _size.z}, material);
            _sides.add(std::make_shared<RTQuad>(sideXZBottom));

            RTQuad sideXZTop(_center - _rad + vec3d(0, _size.y, 0), {_size.x, 0, 0}, {0, 0, _size.z}, material);
            _sides.add(std::make_shared<RN::Transforms::RTFlipFace>(std::make_shared<RTQuad>(sideXZTop)));

            RTQuad sideZYLeft(_center - _rad, {0, 0, _size.z}, {0, _size.y, 0}, material);
            _sides.add(std::make_shared<RTQuad>(sideZYLeft));

            RTQuad sideZYRight(_center - _rad + vec3d(_size.x, 0, 0), {0, 0, _size.z}, {0, _size.y, 0}, material);
            _sides.add(std::make_shared<RTQuad>(sideZYRight));

            RTQuad sideXYBack(_center - _rad, {_size.x, 0, 0}, {0, _size.y, 0}, material);
            _sides.add(std::make_shared<RTQuad>(sideXYBack));

            RTQuad sideXYFront(_center - _rad + vec3d(0,0,_size.z), {_size.x, 0, 0}, {0, _size.y, 0}, material);
            _sides.add(std::make_shared<RTQuad>(sideXYFront));

            _sides_bvh = BVH(_sides);
        }

        RTBox(const RTBox &other): RTDrawable(other), _size(other._size), _center(other._center), _rad(other._rad), _min(other._min), _max(other._max) {
            _bbox = AABB(_center - _rad, _center + _rad);

            RTQuad sideXZBottom(_center - _rad, {_size.x, 0, 0}, {0, 0, _size.z}, material);
            _sides.add(std::make_shared<RTQuad>(sideXZBottom));

            RTQuad sideXZTop(_center - _rad + vec3d(0, _size.y, 0), {_size.x, 0, 0}, {0, 0, _size.z}, material);
            //_sides.add(std::make_shared<RTQuad>(sideXZTop));
            _sides.add(std::make_shared<RN::Transforms::RTFlipFace>(std::make_shared<RTQuad>(sideXZTop)));

            RTQuad sideZYLeft(_center - _rad, {0, 0, _size.z}, {0, _size.y, 0}, material);
            _sides.add(std::make_shared<RTQuad>(sideZYLeft));

            RTQuad sideZYRight(_center - _rad + vec3d(_size.x, 0, 0), {0, 0, _size.z}, {0, _size.y, 0}, material);
            //_sides.add(std::make_shared<RTQuad>(sideZYRight));
            _sides.add(std::make_shared<RN::Transforms::RTFlipFace>(std::make_shared<RTQuad>(sideZYRight)));

            RTQuad sideXYBack(_center - _rad, {_size.x, 0, 0}, {0, _size.y, 0}, material);
            //_sides.add(std::make_shared<RTQuad>(sideXYBack));
            _sides.add(std::make_shared<RN::Transforms::RTFlipFace>(std::make_shared<RTQuad>(sideXYBack)));

            RTQuad sideXYFront(_center - _rad + vec3d(0,0,_size.z), {_size.x, 0, 0}, {0, _size.y, 0}, material);
            _sides.add(std::make_shared<RTQuad>(sideXYFront));

            _sides_bvh = BVH(_sides);
        }

        bool hit(const ray3 &ray, double dmin, double dmax, RTHitInfo &hit_info) const override{
            return _sides_bvh.hit(ray, dmin, dmax, hit_info);
        };

        double pdfValue(const vec3d &origin, const vec3d &direction) const override {
            return _sides.pdfValue(origin, direction);
        }

        vec3d random(const vec3d &origin) const override {
            return _sides.random(origin);
        }

        ~RTBox() = default;

        [[nodiscard]] RTItemType type() const override {return rt_box;}

        vec3d center() const {
            return _center;
        }

        [[nodiscard]] vec3d size() const {
            return _size;
        }

        [[nodiscard]] const AABB &bbox() const override {
            return _bbox;
        }
    private:
        AABB _bbox;
        vec3d _size;
        vec3d _rad;
        vec3d _max;
        vec3d _min;
        vec3d _center;

        RTScene _sides;
        BVH _sides_bvh;
    };

} // RN

#endif //RAYON_RTBOX_H
