//
// Created by root on 11.03.2024.
//

#ifndef RAYLIB_TEMPLATE_SFCIRCLE_H
#define RAYLIB_TEMPLATE_SFCIRCLE_H

#include "SFDrawable.h"

namespace RN {

    class SFCircle : public SFDrawable {
    public:
        float radius;
        glm::vec2 center;

        SFCircle(float r, glm::vec2 c, std::shared_ptr<Material> &mat): SFDrawable(mat), radius(r), center(c){}

        double distance(const glm::vec2 &p) override;
        [[nodiscard]] glm::vec2 normal(const glm::vec2 &p) const override;

        [[nodiscard]] SFDrawableType type() const override {return circle;}

        ~SFCircle() = default;
    };

}


#endif //RAYLIB_TEMPLATE_SFCIRCLE_H
