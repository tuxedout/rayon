//
// Created by root on 11.03.2024.
//

#include "SFCircle.h"

double RN::SFCircle::distance(const glm::vec2 &p) {
    return glm::distance(p, center) - radius;
}

glm::vec2 RN::SFCircle::normal(const glm::vec2 &p) const {
    glm::vec2 normal = p - center;

    normal = glm::normalize(normal);

    return normal;
}