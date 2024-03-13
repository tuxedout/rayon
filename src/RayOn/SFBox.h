//
// Created by root on 12.03.2024.
//

#ifndef RAYLIB_TEMPLATE_SFBOX_H
#define RAYLIB_TEMPLATE_SFBOX_H

#include "SFDrawable.h"

namespace RN {

    class SFBox: public SFDrawable {
    public:

        SFBox(glm::vec2 c, glm::vec2 s, std::shared_ptr<Material> &mat) : SFDrawable(mat), center(c), size(s) {

        }

        glm::vec2 size;
        glm::vec2 center;

        double distance(const glm::vec2 &p) override;
        [[nodiscard]] glm::vec2 normal(const glm::vec2 &p) const override;

        [[nodiscard]] SFDrawableType type() const override {return box;}

        ~SFBox() = default;
    };

} // RN

#endif //RAYLIB_TEMPLATE_SFBOX_H
