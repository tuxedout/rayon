//
// Created by tux on 11.03.2024.
//

#ifndef RAYLIB_TEMPLATE_SCENE_H
#define RAYLIB_TEMPLATE_SCENE_H

#include <vector>
#include "SFDrawable.h"
#include "SFHitInfo.h"
#include "Vec2.h"

#include <memory>

namespace RN {
    class Scene {
    public:
        Scene(int _width_px, int _height_px): width_pix(_width_px), height_pix(_height_px) {
            aspect = static_cast<float>(width_pix) / static_cast<float>(height_pix);

            height = 1.0;
            width = 1.0f * aspect;

            max_coords = {width/2.0f, height / 2.0};
            min_coords = {-width/2.0f, -height / 2.0};
        }

        Scene(const Scene &other){
            height = other.height;
            width = other.width;
            aspect = other.aspect;
            max_coords = other.max_coords;
            min_coords = other.min_coords;

            width_pix = other.width_pix;
            height_pix = other.height_pix;

            children = other.children;
        }

        [[nodiscard]] vec2d sceneCoords(const vec2i &px) const {
            return {
                (px.x - width_pix/2.0)/(height_pix-1),
                (px.y - height_pix/2.0)/(height_pix-1)};
        }

        [[nodiscard]] vec2i pxCoords(const vec2d &scene_coords) const {
            int x = static_cast<int>((scene_coords.x * (height_pix-1)) + (width_pix / 2.0));
            int y = static_cast<int>((scene_coords.y * (height_pix-1)) + (height_pix / 2.0));

            x = std::max(0, std::min(x, width_pix - 1));
            y = std::max(0, std::min(y, height_pix - 1));

            return {x, y};
        }

        void addChild(const std::shared_ptr<SFDrawable>& child) {
            children.push_back(child);
        }

        [[nodiscard]] SFHitInfo hit(const vec2d &p) const;

        [[nodiscard]] bool contains(const vec2d &p) const{
            if ((p.x > max_coords.x) || (p.y > max_coords.y) || (p.x < min_coords.x) || (p.y < min_coords.y)){
                return false;
            }

            return true;
        }

        [[nodiscard]] double distance(const vec2d &p) const;

        [[nodiscard]] vec2i pxSize() const {
            return {width_pix, height_pix};
        }
    protected:
        std::vector<std::shared_ptr<SFDrawable>> children;
        int width_pix;
        int height_pix;

        vec2d max_coords, min_coords;

        double width;
        double height;

        double aspect;
    };
}

#endif //RAYLIB_TEMPLATE_SCENE_H
