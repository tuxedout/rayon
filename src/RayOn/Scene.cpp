//
// Created by root on 11.03.2024.
//

#include "Scene.h"


RN::SFHitInfo &RN::Scene::hit(const glm::vec2 &p, RN::SFHitInfo &hitInfo) const {
    bool first = true;

    glm::vec2 local_p = p;

    for (const auto &item: children){
        if (first) {
            hitInfo.distance = item->distance(local_p);
            hitInfo.item = &*item;
            first = false;
        } else {
            float distance = item->distance(local_p);

            if (distance < hitInfo.distance) {
                hitInfo.distance = distance;
                hitInfo.item = &*item;
            }
        }
    }
    return hitInfo;
}