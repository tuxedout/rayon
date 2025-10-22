//
// Created by tux on 11.03.2024.
//

#include "SFScene.h"

RN::SFPoinInfo RN::SFScene::hit(const vec2d &p) const {
    RN::SFPoinInfo hitInfo;
    hitInfo.distance = std::numeric_limits<float>::max();
    hitInfo.item = nullptr;

    for (const auto &item: children) {
        double distance = item->distance(p);

        if (distance < hitInfo.distance) {
            hitInfo.distance2 = hitInfo.distance;
            hitInfo.distance = distance;

            hitInfo.item2 = hitInfo.item;
            hitInfo.item = item.get();
        }
    }
    return hitInfo;
}

double RN::SFScene::distance(const RN::vec2d &p) const {
    double distance = std::numeric_limits<float>::max();
    double tmp = std::numeric_limits<float>::max();
    for (const auto &item: children) {
        tmp = item->distance(p);

        if (distance > tmp){
            distance = tmp;
        }
    }

    return distance;
}
