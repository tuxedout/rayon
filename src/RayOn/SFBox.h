//
// Created by tux on 12.03.2024.
//

#ifndef RAYLIB_TEMPLATE_SFBOX_H
#define RAYLIB_TEMPLATE_SFBOX_H

#include "SFDrawable.h"

namespace RN {

    class SFBox: public SFDrawable {
    public:

        SFBox(vec2d c, vec2d s, std::shared_ptr<Material> &mat) : SFDrawable(mat), center(c), size(s) {

        }

        SFBox(const SFBox &other): SFDrawable(other){
            center = other.center;
            size = other.size;
        }

        vec2d size;
        vec2d center;

        [[nodiscard]] double distance(const vec2d &p) const override;

        [[nodiscard]] SFDrawableType type() const override {return box;}

        ~SFBox() = default;
    };

} // RN

#endif //RAYLIB_TEMPLATE_SFBOX_H
