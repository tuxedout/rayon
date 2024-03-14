//
// Created by root on 14.03.2024.
//

#ifndef RAYON_CPP_CMAKE_INTERVAL_H
#define RAYON_CPP_CMAKE_INTERVAL_H

namespace RN {
    template<typename T>
    class Interval {
    public:
        Interval(T v, T mi, T ma, T st): value(v), min_value(mi), max_value(ma), value_step(st){}

        T getValue() const {
            return value;
        }

        T stepUp() {
            value = (value + value_step <= max_value) ? value + value_step : max_value;
            return value;
        }

        T stepDown() {
            value = (value - value_step >= min_value) ? value - value_step : min_value;
            return value;
        }

        void setValue(T v) {
            value = v;
        }
    private:
        T value;
        T min_value;
        T max_value;
        T value_step;
    };

} // RN

#endif //RAYON_CPP_CMAKE_INTERVAL_H
