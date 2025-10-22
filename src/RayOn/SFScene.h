/*
 *
 *       Created by tux on 11.03.2024.
 *       ________   _______  ____ ____  _______  ____ ____
 *      │----R---\ /---A---\ ----Y---- /---O---\│----N----\
 *      │         │         │    │    │         │         │
 *      │    ^    │    ^    │    │    │    ^    │    ^    │
 *      │    │    │    │    │    │    │    │    │    │    │
 *      │    │    │    │    │    │    │    │    │    │    │
 *      │    ┼    │    ┼    │    ┼    │    ┼    │    │    │
 *      │        (          \         │    │    │    │    │
 *      │    ^    │    ^    │)        │    │    │    │    │
 *      │    │    │    │    /         │    v    │    │    │
 *      │    │    │    │    │        /│         │    │    │
 *      │────│────│────│────│───────/  \_______/│____│____│
 *
 *      RayOn - simple rig to play with rays
 *
 *      SFScene contains vector of drawable objects,
 *      takes care
 *
 */
#ifndef RAYON_SFSCENE_H
#define RAYON_SFSCENE_H

#include <vector>
#include "SFDrawable.h"
#include "SFPoinInfo.h"
#include "Vec2.h"

#include <memory>

namespace RN {
    class SFScene {
    public:
        SFScene& operator=(const SFScene& other) {
            if (this != &other) {
                // Direct copy of simple types and objects that support copy assignment
                width_px = other.width_px;
                height_px = other.height_px;
                aspect = other.aspect;
                width = other.width;
                height = other.height;
                max_coords = other.max_coords;
                min_coords = other.min_coords;

                // For container types like std::vector, copying the entire container is usually what you want,
                // but consider if deep copy of pointed-to objects is needed instead.
                // Here, std::shared_ptr will handle the reference counting automatically.
                children = other.children;
            }
            return *this;
        }


        SFScene(int _width_px, int _height_px): width_px(_width_px), height_px(_height_px) {
            aspect = static_cast<float>(width_px) / static_cast<float>(height_px);

            height = 1.0;
            width = 1.0f * aspect;

            // coordinate limits used to prevent sampling outside of screen e.g. not to run out of SDFLUT table bounds
            max_coords = {width/2.0f, height / 2.0};
            min_coords = {-width/2.0f, -height / 2.0};
        }

        SFScene(const SFScene &other){
            height = other.height;
            width = other.width;
            aspect = other.aspect;
            max_coords = other.max_coords;
            min_coords = other.min_coords;

            width_px = other.width_px;
            height_px = other.height_px;

            children = other.children;
        }

        [[nodiscard]] vec2d sceneCoords(const vec2i &px) const {
            return {
                    (0.5+px.x - (float)width_px / 2.0) / height_px,
                    (0.5+px.y - (float)height_px / 2.0) / height_px
            };
        }

        [[nodiscard]] vec2i pxCoords(const vec2d &scene_coords) const {
            int x = static_cast<int>((scene_coords.x * (height_px)) + (width_px / 2.0)-0.5);
            int y = static_cast<int>((scene_coords.y * (height_px)) + (height_px / 2.0)-0.5);

            x = std::max(0, std::min(x, width_px - 1));
            y = std::max(0, std::min(y, height_px - 1));

            return {x, y};
        }

        void addChild(const std::shared_ptr<SFDrawable>& child) {
            children.push_back(child);
        }

        [[nodiscard]] SFPoinInfo hit(const vec2d &p) const;

        [[nodiscard]] bool contains(const vec2d &p) const{
            if ((p.x > max_coords.x) || (p.y > max_coords.y) || (p.x < min_coords.x) || (p.y < min_coords.y)){
                return false;
            }

            return true;
        }

        [[nodiscard]] double distance(const vec2d &p) const;

        [[nodiscard]] vec2i pxSize() const {
            return {width_px, height_px};
        }
    protected:
        std::vector<std::shared_ptr<SFDrawable>> children;


        int width_px;
        int height_px;

        // coordinate limits used to prevent sampling outside of screen e.g. not to run out of SDFLUT table bounds
        vec2d max_coords, min_coords;

        double width;
        double height;

        double aspect;
    };
}

#endif //RAYON_SFSCENE_H
