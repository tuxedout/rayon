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
 *      RTDrawable.h
 *
 */

#ifndef RAYON_RTDRAWABLE_H
#define RAYON_RTDRAWABLE_H

#include <memory>
#include <utility>

#include "../Vec2.h"
#include "../Ray3.h"
#include "../Materials/Material.h"
#include "RTHitInfo.h"
#include "AABB.h"

namespace RN {
    enum RTItemType {
        // forms
        rt_item,
        rt_generic,
        rt_sphere,
        rt_box,
        rt_disc,
        rt_quad,
        rt_ellipsoid,
        rt_cylinder,
        rt_line,
        rt_bounding_box_wireframe,
        // containers
        rt_bvh,
        rt_scene,
        rt_lightslist,
        // transforms
        rt_rotate,
        rt_translate,
        rt_flipface,
        rt_scale,
    };



    class RTItem {
    public:

        RTItem() = default;

//        explicit RTDrawable(const std::shared_ptr<RN::Materials::Material> &mat): material(mat){
//            material = mat;
//        }

        RTItem(const RTItem &other) = default;

        virtual bool hit(const ray3 &ray, double dmin, double dmax, RTHitInfo &hit_info) const = 0;

        ~RTItem() = default;

        [[nodiscard]] virtual RTItemType type() const {return rt_item;}

//        std::shared_ptr<RN::Materials::Material> material;

        [[nodiscard]] virtual const AABB &bbox() const = 0;

        virtual double pdfValue(const vec3d &origin, const vec3d &direction) const {
            return 0.0;
        }

        virtual vec3d random(const vec3d &origin) const {
            return {1, 0, 0};
        }

    };

    class RTDrawable: public RTItem{
    public:
        explicit RTDrawable(const std::shared_ptr<RN::Materials::Material> &mat): material(mat){
            material = mat;
        }

        std::shared_ptr<RN::Materials::Material> material;

        [[nodiscard]] virtual RTItemType type() const {return rt_generic;}

        vec3d color;
    };
}

#endif //RAYON_RTDRAWABLE_H
