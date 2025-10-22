/*
 *
 *       Created by tux on 23.03.2024.
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
 *      DiscreteInterval.h
 *
 */
#include <vector>
#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>

#ifndef RAYON_DISCRETEINTERVAL_H
#define RAYON_DISCRETEINTERVAL_H

namespace RN {

    template<typename T>
    class DiscreteInterval {
    private:
        std::vector<T> values;
        std::default_random_engine engine;

    public:
        // Конструктор принимает вектор значений
        DiscreteInterval(const std::vector<T>& values) : values(values) {
             unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
//            unsigned seed = 42;
            engine.seed(seed);
            // Для обеспечения корректности операций, вектор должен быть отсортирован
            std::sort(this->values.begin(), this->values.end());
        }

        // Получить случайное значение из дискретного интервала
        T &getRandom() {
            std::uniform_int_distribution<size_t> dist(0, values.size() - 1);
            return values[dist(engine)];
        }

        // Получить значение, соответствующее доле интервала от 0 до 1
        T getAt(double proportion) const {
            if (values.empty()) {
                throw std::logic_error("The interval is empty.");
            }
            size_t index = static_cast<size_t>(proportion * (values.size() - 1));
            return values[index];
        }

        // Вывести значения интервала
        void print() const {
            std::cout << "Discrete interval values: ";
            for (const auto& val : values) {
                std::cout << val << " ";
            }
            std::cout << std::endl;
        }
    };
} // RN

#endif //RAYON_DISCRETEINTERVAL_H
