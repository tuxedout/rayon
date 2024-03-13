//
// Created by root on 12.03.2024.
//

#include "SFBox.h"

namespace RN {

    double SFBox::distance(const glm::vec2 &p) {
        glm::vec2 srv_d = glm::abs(center - p) - size;

        srv_d.x = (srv_d.x < 0.0) ? 0.0f : srv_d.x;
        srv_d.y = (srv_d.y < 0.0) ? 0.0f : srv_d.y;

        return fmin(fmax(srv_d.x, srv_d.y), 0.0) + glm::length(srv_d);
    }

    glm::vec2 SFBox::normal(const glm::vec2 &p) const {
        glm::vec2 localPoint = p - center;

        glm::vec2 delta = size * 0.5f;
        delta = glm::abs(localPoint) - delta;


        if (delta.x > delta.y) {
            if (localPoint.x > 0) {
                return {1, 0};
            } else {
                return {-1, 0};
            }
        } else {
            if (localPoint.y > 0) {
                return {0, 1};
            } else {
                return {0, -1};
            }
        }
    }
} // RN