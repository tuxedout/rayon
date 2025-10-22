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
    if (IsKeyPressed(KEY_I)) {
        ShowInfo();
    }
}

void RN::RayOnApp::ShowInfo() {
    std::stringstream top_info;
    top_info << "SPP: " << settings.SamplesPerPixel() << ", max steps: "<< settings.MaxSteps() << ", max dist: " << settings.MaxDistance() << ", epsilon: " << settings.Epsilon() << ", depth: " << settings.MaxDepth();
    ShowMessage(top_info.str());
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
    proceedMessages();
}

void RN::RayOnApp::ShowMessage(const std::string &m) {
    TextMessage msg_obj;

    msg_obj.message = m;
    msg_obj.ttl = MESSAGE_DURATION;
    msg_obj.color = RED;

    messages_pool.push_back(msg_obj);
}

void RN::RayOnApp::proceedMessages() {
    unsigned int row = 0;
    // iteration in reverse order, to safely delete messages
    for (int i = messages_pool.size() - 1; i >= 0; --i) {
        //
        messages_pool[i].ttl -= GetFrameTime();

        // if ttl is ended then bye-bye message!
        if (messages_pool[i].ttl <= 0) {
            messages_pool.erase(messages_pool.begin() + i);
        } else {
            DrawText(messages_pool[i].message.c_str(), 10, 10 + row * MESSAGE_ROW_HEIGHT, MESSAGE_FONT_SIZE, messages_pool[i].color);
            row++;
        }
        // if maximum messages count shown then don't proceed others
        if (row >= MESSAGE_ROWS_MAX){
            return;
        }
    }
}
