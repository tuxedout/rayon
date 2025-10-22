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
 *      Image.h - texture from raster image
 */

// TODO: fix it, works only in debug builds

#ifndef RAYON_IMAGE_TEXTURE_H
#define RAYON_IMAGE_TEXTURE_H

#include <iostream>
#include <memory>
#include <optional>
#include <algorithm> // Include for std::copy
#include "../Vec3.h"
#include "../RayOn.h"
#include "Texture.h"
#include "external/stb_image.h"

namespace RN::Textures {

    class ImageTexture: public RN::Textures::Texture {
    public:
        ImageTexture(int w, int h, int ch, unsigned char *d) : width(w), height(h), channels(ch), data(d) {}

        friend std::ostream& operator<<(std::ostream& os, const ImageTexture& img);

        // Конструктор копирования для глубокого копирования, использующий std::copy
        ImageTexture(const ImageTexture& other) : width(other.width), height(other.height), channels(other.channels) {
            size_t dataSize = width * height * channels;
            data = new unsigned char[dataSize];
            std::copy(other.data, other.data + dataSize, data);
        }

        // Оператор присваивания для глубокого копирования, использующий std::copy
        ImageTexture& operator=(const ImageTexture& other) {
            if (this != &other) { // проверка на самоприсваивание
                width = other.width;
                height = other.height;
                channels = other.channels;
                size_t dataSize = width * height * channels;
                unsigned char* newData = new unsigned char[dataSize];
                std::copy(other.data, other.data + dataSize, newData);
                delete[] data; // Освобождаем старую память
                //stbi_image_free(data);
                data = newData;
            }
            return *this;
        }

        static std::optional<ImageTexture> Load(const std::string &file_name) {
            std::cout << "loading file to texture:" << "\n";
            std::cout << file_name << "\n";
            int _width, _height, _channels;
            unsigned char *img = stbi_load(file_name.c_str(), &_width, &_height, &_channels, 0);
            if (img == nullptr) {
                std::cerr << "Error loading image: " << stbi_failure_reason() << "\n";
                return std::nullopt; // Возвращаем пустое значение
            }
            std::cout << "texture loaded w:" << _width << " h:" << _height << " c:" << _channels << "\n";
            return ImageTexture(_width, _height, _channels, img);
        }

        virtual ~ImageTexture() {
//            std::cout << "free image\n";
//            std::cout << *this;
            stbi_image_free(data);
        }

        virtual const vec3d color(const vec2d &uv, const vec3d &p) override {
            if (data == nullptr) {
                std::cout << "No data\n";
                return {0}; // Возвращаем черный цвет, если данных нет
            }
            int x = std::clamp((int)(uv.x * width), 0, width - 1);
            int y = std::clamp((int)(uv.y * height), 0, height - 1);
            int pixelIndex = (x + y * width) * channels;
            double r = data[pixelIndex] / 255.0f;
            double g = (channels > 1) ? data[pixelIndex + 1] / 255.0f : r;
            double b = (channels > 2) ? data[pixelIndex + 2] / 255.0f : g;

            vec3d tmp(r,g,b);

            return tmp;
        }

    private:
        unsigned char *data;

        int width;
        int height;
        int channels;
    };

} // namespace RN::Textures

#endif //RAYON_IMAGE_TEXTURE_H

