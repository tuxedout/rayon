//
// Created by root on 17.03.2024.
//

#ifndef RAYLIB_TEMPLATE_SEGMENT_H
#define RAYLIB_TEMPLATE_SEGMENT_H

#include <cstdio>
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>
#include <filesystem>
#include <fstream>
#include <thread>
#include "raylib.h"

struct Segment {
    int x, y, width, height, screen_width, screen_height;
    Color* data;
    std::chrono::time_point<std::chrono::high_resolution_clock> end_time = std::chrono::high_resolution_clock::now();

    Segment() : x(0), y(0), width(0), height(0), screen_width(0), screen_height(0), data(nullptr) {}

    Segment(const Segment& other)
            : x(other.x), y(other.y), width(other.width), height(other.height), screen_width(other.screen_width), screen_height(other.screen_height), data(nullptr) {
        if (other.data) {
            data = new Color[width * height];
            std::copy(other.data, other.data + width * height, data);
        }
    }

    Segment& operator=(const Segment& other) {
        if (this != &other) {

            delete[] data;

            x = other.x;
            y = other.y;
            width = other.width;
            height = other.height;
            screen_width = other.screen_width;
            screen_height = other.screen_height;
            if (other.data) {
                data = new Color[width * height];
                std::copy(other.data, other.data + width * height, data);
            }
        }
        return *this;
    }

    ~Segment() {
        delete[] data;
    }
};


#endif //RAYLIB_TEMPLATE_SEGMENT_H
