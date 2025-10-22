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
 *      RayOn.cpp
 *
 */
#include "RayOn.h"
#include <immintrin.h>

namespace RN {
    const double PI_D = 3.1415926535897932384626433832795;
    const double PI_PI_D = 3.1415926535897932384626433832795 * 2.0;
    const double EPSILON_D = 0.00001;//3.1415926535897932384626433832795 / 1000;
    const double EPSILON_100_D = 3.1415926535897932384626433832795 / 100;

    auto rn_seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::uniform_real_distribution<double> _rn_distribution(0,1);
    std::random_device  _rn_generator;

    double random() {
        unsigned long long random_number;
        //if (_rdrand64_step(&random_number)) {
//        _rdrand64_step(&random_number);
//            // Преобразуем случайное целое число в диапазоне от 0 до максимального значения unsigned long long
//            // в число типа double в диапазоне от 0 до 1
//            return static_cast<double>(random_number) / std::numeric_limits<unsigned long long>::max();
//        } else {
//            throw std::runtime_error("Ошибка генерации случайного числа");
//        }
        return _rn_distribution(_rn_generator);
    }

    double random(const double &min, const double &max) {
        return min + random() * (max - min);
    }

    int random_int(int min, int max) {
        // Use floor to convert double to int and guarantee reaching max
        return static_cast<int>(std::floor(random(min, static_cast<double>(max+1))));
    }
}