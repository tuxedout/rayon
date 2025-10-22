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
 *      Interval.h
 *
 */

#ifndef RAYON_INTERVAL_H
#define RAYON_INTERVAL_H

#include <iostream>
#include <random>
#include <chrono>

namespace RN {
    template<typename T>
    class Interval {
    private:
        // starting and ending points of interval
        T start;
        T end;

        // random related stuff
        std::mt19937_64 engine;
        std::uniform_real_distribution<T> dist;

    public:
        Interval(T start, T end) : start(start), end(end), dist(start, end) {
            updateThings();
        }

        Interval(const Interval<T> &a, const Interval<T> &b) {
            start = std::min(a.start, b.start);
            end = std::max(a.end, b.end);

            updateThings();
        }

        Interval(): start(0), end(0) {
            updateThings();
        }

        T size() const {
            return start-end;
        }

        // include another interval
        void include(const Interval<T> a) {
            start = std::min(a.start, start);
            end =   std::max(a.end, end);
        }

        // expand to include value
        void expand(T value) {
            start -= value;
            end -= value;
        }

        //
        bool contains(T a) const {
            return (start < a) && (a < end);
        }

        bool overlaps(const Interval<T> &other) {
            return contains(other.start) || contains(other.end);
        }

        // clamp value
        T clamp(T a) {
            if (a < start) {
                return start;
            }
            if (a > end) {
                return end;
            }

            return a;
        }

        // get starting point of interval
        T getStart() const {
            return start;
        }

        // get ending point of interval
        T getEnd() const {
            return end;
        }

        // set starting point
        void setStart(T s) {
            start = s;
            updateThings();
        }

        // set ending point
        void setEnd(T end) {
            this->end = end;
            updateThings();
        }

        // get random number from interval
        T getRandom() {
            return dist(engine);
        }

        // get value from interval with position proportional to value from 0(starting point) to 1(ending point)
        T getAt(T proportion) const {
            return start + proportion * (end - start);
        }

        // summation (shift to right)
        Interval operator+(T value) const {
            return Interval(start + value, end + value);
        }

        // summation (shift to left)
        Interval operator-(T value) const {
            return Interval(start - value, end - value);
        }

        Interval &operator=(const Interval &other) {
            if (this != &other) {
                start = other.start;
                end = other.end;
                updateThings();
            }
            return *this;
        }

        // print interval
        void print() const {
            std::cout << "Interval: [" << start << ", " << end << "]" << std::endl;
        }

    private:
        // update entropy generation stuff
        void updateThings() {
            dist = std::uniform_real_distribution<T>(start, end);
            auto seed = std::chrono::steady_clock::now().time_since_epoch().count();
            //auto seed = 42;
            engine.seed(seed);
        }
    };


} // RN

#endif //RAYON_INTERVAL_H
