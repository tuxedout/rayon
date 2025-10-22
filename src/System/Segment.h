/*
 *
 *       Created by tux on 17.03.2024.
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
 *      Segment structure that contains data needed to define rendering task and represent it's results
 *
 */
#ifndef RAYON_SEGMENT_H
#define RAYON_SEGMENT_H

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
        // segment pixel position in screen coordinates
    int x,y,
        // segment pixel size
        width,
        height,
        // needed for renderer
        // TODO: manage to remove them
        screen_width, screen_height;

    // rendering results
    Color* data;

    // task finish time
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


#endif //RAYON_SEGMENT_H