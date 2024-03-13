//
// Created by root on 11.03.2024.
//

#ifndef RAYLIB_TEMPLATE_SFHITINFO_H
#define RAYLIB_TEMPLATE_SFHITINFO_H

#include "Material.h"
#include "SFDrawable.h"
#include <memory>

namespace RN {
    class SFHitInfo {
    public:
        SFHitInfo(): distance(0), item(nullptr){};
        SFHitInfo(float d, SFDrawable *i): distance(d), item(i){};

        SFHitInfo(const SFHitInfo &hi) = default;

        float distance;

        SFDrawable *item;
    };
}


#endif //RAYLIB_TEMPLATE_SFHITINFO_H
