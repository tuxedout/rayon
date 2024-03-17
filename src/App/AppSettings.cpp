//
// Created by tux on 14.03.2024.
//

#include "AppSettings.h"

namespace RN {
    void AppSettings::loadSettings() {
        std::ifstream settingsFile(settings_file_path);
        if (settingsFile.is_open()) {
            nlohmann::json settingsJson;
            settingsFile >> settingsJson;
            samples_per_pixel = settingsJson.value("samples_pex_pixel", samples_per_pixel);

            double _bias = RN_APP_BIAS;
            _bias = settingsJson.value("bias", _bias);
            bias.setValue(_bias);

            double _max_distance = RN_APP_MAX_DISTANCE;
            _max_distance = settingsJson.value("max_distance", _max_distance);
            max_distance.setValue(_max_distance);

            int _max_depth = RN_APP_MAX_DEPTH;
            _max_depth = settingsJson.value("max_depth", _max_depth);
            max_depth.setValue(_max_depth);

            int _max_steps = RN_APP_MAX_STEPS;
            _max_steps = settingsJson.value("max_steps", _max_steps);
            max_steps.setValue(_max_steps);

            double _epsilon = RN_APP_EPSILON;
            _epsilon = settingsJson.value("epsilon", _epsilon);
            epsilon.setValue(_epsilon);
        }
    }

    void AppSettings::saveSettings() {
        std::ofstream settingsFile(settings_file_path);
        if (settingsFile.is_open()) {
            nlohmann::json settingsJson;
            settingsJson["samples_per_pixel"] = samples_per_pixel;
            settingsJson["bias"] = bias.getValue();
            settingsJson["max_distance"] = max_distance.getValue();
            settingsJson["max_depth"] = max_depth.getValue();
            settingsJson["max_steps"] = max_steps.getValue();
            settingsJson["epsilon"] = epsilon.getValue();
            settingsFile << settingsJson.dump(4);
            settingsFile.close();
        }
    }
} // RN