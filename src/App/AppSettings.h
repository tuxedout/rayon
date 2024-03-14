//
// Created by root on 14.03.2024.
//

#ifndef RAYON_CPP_CMAKE_APPSETTINGS_H
#define RAYON_CPP_CMAKE_APPSETTINGS_H


#include <vector>
#include "../json.hpp"
#include <fstream>
#include <vector>
#include "../Misc/Interval.h"

const double RN_APP_MAX_DISTANCE = 15.0;
const int RN_APP_SAMPLES_P_PIX = 16;
const int RN_APP_MAX_STEPS = 10;
const int RN_APP_MAX_DEPTH = 5;
const double RN_APP_EPSILON = 0.0001;
const double RN_APP_BIAS = 0.000001;

namespace RN {

    class AppSettings {
    public:

        AppSettings() = default;


        ~AppSettings() = default;

        AppSettings(const AppSettings &) = default;

        AppSettings &operator=(const AppSettings &) = default;

        AppSettings(AppSettings &&) = default;

        AppSettings &operator=(AppSettings &&) = default;

        static AppSettings &getInstance() {
            static AppSettings instance;
            return instance;
        }

        [[nodiscard]] int SamplesPerPixel() const{
            return samples_per_pixel;
        }

        [[nodiscard]] int MaxDepth() const{
            return max_depth.getValue();
        }

        [[nodiscard]] double MaxDistance() const{
            return max_distance.getValue();
        }

        [[nodiscard]] double Epsilon() const{
            return epsilon.getValue();
        }

        [[nodiscard]] double Bias() const{
            return bias.getValue();
        }

        int MaxStepsUp() {
            return max_steps.stepUp();
        }

        int MaxStepsDown() {
            return max_steps.stepDown();
        }

        void loadSettings();
        void saveSettings();
    private:
        const std::string settings_file_path = "settings.json";

        int samples_per_pixel = RN_APP_SAMPLES_P_PIX;
        std::vector<int> samplesOptions = {4, 8, 16, 32, 64, 128, 256, 512, 1024};

        Interval<int> max_steps = {RN_APP_MAX_STEPS, 1, 256, 1};
        Interval<int> max_depth = {RN_APP_MAX_DEPTH, 0, 30, 1};
        Interval<double> max_distance = {RN_APP_MAX_DISTANCE, 0.1, 10.0, 0.1};
        Interval<double> epsilon = {RN_APP_EPSILON, 0.0, 0.1, 0.0001};
        Interval<double> bias = {RN_APP_BIAS, 0.0, 0.001, 0.0000001};
    };

} // RN

#endif //RAYON_CPP_CMAKE_APPSETTINGS_H
