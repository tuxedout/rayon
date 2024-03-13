//
// Created by root on 11.03.2024.
//

#ifndef RAYLIB_TEMPLATE_SCENE_H
#define RAYLIB_TEMPLATE_SCENE_H

#include <vector>
#include "SFDrawable.h"
#include "SFHitInfo.h"

#include <memory>

namespace RN {
    class Scene {
        std::vector<std::shared_ptr<SFDrawable>> children;

    public:
        void addChild(const std::shared_ptr<SFDrawable>& child) {
            children.push_back(child);
        }

        SFHitInfo &hit(const glm::vec2 &p, SFHitInfo &hitInfo) const;
    };
}

#endif //RAYLIB_TEMPLATE_SCENE_H
