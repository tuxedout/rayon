//
// Created by root on 11.03.2024.
//

#ifndef RAYLIB_TEMPLATE_SFDRAWABLE_H
#define RAYLIB_TEMPLATE_SFDRAWABLE_H

#include <glm/vec2.hpp>
#include <glm/glm.hpp>
#include "Material.h"
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

        virtual double distance(const glm::vec2 &p) = 0;

        ~SFDrawable() = default;

        [[nodiscard]] virtual glm::vec2 normal(const glm::vec2 &p) const = 0;
        [[nodiscard]] virtual SFDrawableType type() const {return generic;}

        std::shared_ptr<Material> material;
    };
}


#endif //RAYLIB_TEMPLATE_SFDRAWABLE_H
