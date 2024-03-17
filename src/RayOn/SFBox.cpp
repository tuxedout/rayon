//
// Created by tux on 12.03.2024.
//

#include "SFBox.h"

namespace RN {
    double SFBox::distance(const vec2d &p) const {

        RN::vec2d srv_d;

        srv_d = (center - p).abs() - size;

        if (srv_d.x < 0) {
            srv_d.x = 0ll;
        }

        if (srv_d.y < 0) {
            srv_d.y = 0ll;
        }

        return fmin(fmax(srv_d.x, srv_d.y), 0.0) + srv_d.length();
    }
} // RN