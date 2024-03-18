/*
 *
 *       Created by tux on 14.03.2024.
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
 *      RayOnApp - application class to provide utility funcs.
 *
 */

#include "RayOnApp.h"
#include "raylib.h"
#include <sstream>

void RN::RayOnApp::Init() {
    EnsureScreenshotsDirExists();

    settings.loadSettings();

    mode = initialized;
}

void RN::RayOnApp::HandleUserInput() {
    if (IsKeyPressed(KEY_S)) {
        MakeScreenshot();
    }
}

void RN::RayOnApp::ShowInfo() {
    std::stringstream top_info;
    top_info << "SPP: " << settings.SamplesPerPixel() << ", max steps: "<< settings.MaxSteps() << ", max dist: " << settings.MaxDistance() << ", epsilon: " << settings.Epsilon();
}

std::string RN::RayOnApp::GenerateScreenshotFileName() {
    std::time_t now = std::time(nullptr);

    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", std::localtime(&now));

    std::string fileName = SCREENSHOTS_FOLDER + "/screenshot_" + std::string(buf) + ".png";
    return fileName;
}

void RN::RayOnApp::EnsureScreenshotsDirExists() {
    std::filesystem::path dirPath{SCREENSHOTS_FOLDER};
    if (!std::filesystem::exists(dirPath)) {
        std::filesystem::create_directories(dirPath);
    }
}

void RN::RayOnApp::MakeScreenshot() {
    std::string file_name = GenerateScreenshotFileName();
    TakeScreenshot(file_name.c_str());

    ShowMessage(file_name + " saved!");
}

void RN::RayOnApp::Run() {
    if (show_message) {
        message_time_left -= GetFrameTime();
        if (message_time_left <= 0.0f) {
            show_message = false;
        }
    }

    if (show_message) {
        DrawText(message_text.c_str(), 10, 10, 20, RED);
    }
}

void RN::RayOnApp::ShowMessage(const std::string m) {
    message_text = m;
    show_message = true;
    message_time_left = MESSAGE_DURATION;
}