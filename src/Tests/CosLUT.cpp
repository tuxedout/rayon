/*
 *
 *       Created by tux on 08.06.2024.
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
 *      CosLUT.cpp
 *
 */

#include <iostream>
#include "../RayOn/CMS/CosLUT.h"

int main() {
    int pointsPerQuarterCircle = 90; // Выберите количество точек на четверть окружности
    RN::CosLUT cosLut(pointsPerQuarterCircle);

    int N = 10000000;

    double error = 0;
    double sum = 0;

    auto start = std::chrono::high_resolution_clock::now();  // start timer

    for (int i=0; i < N; i++) {

        double angle = RN::random() * RN::PI_PI_D;

        double cos_func = cos(angle);

        sum += cos_func;
    }

    auto end = std::chrono::high_resolution_clock::now();  // end timer

    auto total_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "func time taken: " << total_duration << std::endl;
    std::cout << "sum: " << sum << std::endl;

    sum = 0;

    start = std::chrono::high_resolution_clock::now();  // start timer

    for (int i=0; i < N; i++) {

        double angle = RN::random() * RN::PI_PI_D;

        double cos_func = RN::LUTs::cos.get(angle);

        sum += cos_func;
    }

    end = std::chrono::high_resolution_clock::now();  // end timer

    total_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "LUT time taken: " << total_duration << std::endl;
    std::cout << "sum: " << sum << std::endl;

    return 0;
}