//
//       Created by tux on 11.03.2024.
//       ________   _______  ____ ____  _______  ____ ____
//      │----R---\ /---A---\ ----Y---- /---O---\│----N----\
//      │         │         │    │    │         │         │
//      │    ^    │    ^    │    │    │    ^    │    ^    │
//      │    │    │    │    │    │    │    │    │    │    │
//      │    │    │    │    │    │    │    │    │    │    │
//      │    ┼    │    ┼    │    ┼    │    ┼    │    │    │
//      │        (          \         │    │    │    │    │
//      │    ^    │    ^    │)        │    │    │    │    │
//      │    │    │    │    /         │    v    │    │    │
//      │    │    │    │    │        /│         │    │    │
//      │────│────│────│────│───────/  \_______/│____│____│
//                                                                                                                                                
//      RayOn - simple rig to play with rays
//                                                                                                                                                
//                                                                                                                                                

#ifndef RAYLIB_TEMPLATE_SFDRAWABLE_H
#define RAYLIB_TEMPLATE_SFDRAWABLE_H

#include "Material.h"
#include "Vec2.h"
#include <memory>

namespace RN {

    enum SFDrawableType {
        generic,
        circle,
        box
    };

    class SFDrawable {
    public:
        explicit SFDrawable(std::shared_ptr<Material> &mat): material(mat){}

        SFDrawable(const SFDrawable &other){
            material = other.material;
        }

        virtual double distance(const vec2d &p) const = 0;

        ~SFDrawable() = default;

        [[nodiscard]] virtual SFDrawableType type() const {return generic;}

        std::shared_ptr<Material> material;
    };
}


#endif //RAYLIB_TEMPLATE_SFDRAWABLE_H
