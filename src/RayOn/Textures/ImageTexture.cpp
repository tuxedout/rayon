/*
 *
 *       Created by tux on 19.04.2024.
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
 *      Image.cpp
 *
 */
#include <cmath> // Для функции std::round
#include <ostream>
#include "ImageTexture.h"

//namespace RN::Textures {
//
//    std::ostream& operator<<(std::ostream& os, const Image& img) {
//        os << "Image (Width: " << img.width << ", Height: " << img.height << ", Channels: " << img.channels << ")\n";
//        // Вывод изображения в консоль
//        for (int y = 0; y < img.height; y++) {
//            for (int x = 0; x < img.width; x++) {
//                int pixelIndex = (x + y * img.width) * img.channels;
//                float grey = 0;
//                // Среднее значение по каналам для получения оттенков серого
//                for (int c = 0; c < img.channels; c++) {
//                    grey += img.data[pixelIndex + c];
//                }
//                grey /= img.channels;
//                // Нормализация
//                grey = (grey / 255.0f) * 100.0f;
//                // Выбор символа в зависимости от яркости
//                os << (grey > 50 ? "█" : " ");
//            }
//            os << "\n";
//        }
//        return os;
//    }
//
//} // namespace RN::Textures