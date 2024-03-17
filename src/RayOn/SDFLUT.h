#ifndef RAYLIB_TEMPLATE_SDFLUT_H
#define RAYLIB_TEMPLATE_SDFLUT_H

#include <vector>
#include "Vec2.h"
#include "Scene.h"

namespace RN {

    class SDFLUT {
    public:
        SDFLUT(const Scene &scene): scene(scene) {
            setSize(scene.pxSize());
            generate();
        }

        void setValue(const SFHitInfo &val, vec2i p){
            table[p.y * width + p.x] = val; // Исправлено с p.y на p.x
        }

        [[nodiscard]] const SFHitInfo* getValue(vec2d p) const {
            vec2i px = scene.pxCoords(p);

            return &table[px.y * width + px.x];
        }

        [[nodiscard]] SFHitInfo *getValue(vec2d p) {
            vec2i px = scene.pxCoords(p);
            return &table[px.y * width + px.x];
        }

        void generate() {
            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    vec2d scene_coords = scene.sceneCoords({x, y});
                    SFHitInfo hitInfo = scene.hit(scene_coords); // Предполагая, что scene.hit возвращает SFHitInfo
                    setValue(hitInfo, {x, y});
                }
            }
        }

        void setSize(vec2i s){
            width = s.x;
            height = s.y;
            table.resize(width * height); // Предварительное выделение места под все элементы
        }

        [[nodiscard]] vec2d normal(const vec2d &p) const {
            const double delta = 0.00001; // Малое смещение для вычисления градиента
            const double twoDelta = 2 * delta;

            // Вычисляем компоненты градиента по X и Y, используя центральную разность
            double gradX = (getValue(vec2d {p.x + delta, p.y})->distance - getValue(vec2d {p.x - delta, p.y})->distance) / twoDelta;
            double gradY = (getValue(vec2d {p.x, p.y + delta})->distance - getValue(vec2d {p.x, p.y - delta})->distance) / twoDelta;

            vec2d grad = {gradX, gradY};
            grad.normalize();
            return grad;
        }
    private:
        const Scene &scene;
        std::vector<SFHitInfo> table;
        int width = 0;
        int height = 0;
    };

} // namespace RN

#endif //RAYLIB_TEMPLATE_SDFLUT_H