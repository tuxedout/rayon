/*
 *
 *       Created by tux on 28.03.2024.
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
 *      CheckeredTexture.h
 *
 */

#ifndef RAYON_CHECKEREDCOLOR_H
#define RAYON_CHECKEREDCOLOR_H

#include "CheckeredTexture.h"


    namespace RN::Textures {

        class CheckeredColorTexture: public CheckeredTexture {
        public:
            double scale = PI_D*200;

            CheckeredColorTexture() {

            }

            // Функция для генерации уникального цвета для каждого квадрата.
            static vec3d generateUniqueColor(int x, int y) {
                // Набор предопределенных цветов
                static const std::vector<vec3d> colors = {
                        {1.0, 1.0, 0.0},
                        {1.0, 0.0, 1.0}, // Синий
                        {1.0, 1.0, 0.0}, // Желтый
                        {0.0, 1.0, 1.0}, // Голубой
                        {1.0, 0.0, 1.0}, // Фиолетовый
                        {1.0, 0.5, 0.0}  // Оранжевый
                };

                // Хэш-функция для x и y, простой вариант - сумма с коэффициентом
                int index = (x * 17 + y * 13) % colors.size();

                return colors[index];
            }

            const vec3d color(const vec2d &uv, const vec3d &p) override {
                auto tmp_uv = uv * scale;

                int x = static_cast<int>(std::round(tmp_uv.x));
                int y = static_cast<int>(std::round(tmp_uv.y));

                // Используем x и y для выбора уникального цвета из набора.
                return generateUniqueColor(x, y);
            }
        };


    } // Textures
// RN

#endif //RAYON_CHECKEREDCOLOR_H