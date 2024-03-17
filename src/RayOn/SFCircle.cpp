//
// Created by tux on 11.03.2024.
//

#include "SFCircle.h"

double RN::SFCircle::distance(const RN::vec2d &p) const {
    return vec2d::distance(p, center) - radius;
}