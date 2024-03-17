/*
 *
 *       Created by tux on 11.03.2024.
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

#ifndef RAYON_CPP_CMAKE_RAYONAPP_H
#define RAYON_CPP_CMAKE_RAYONAPP_H

#include "AppSettings.h"

namespace RN {
    const std::string SCREENSHOTS_FOLDER = "screenshots";
    const float MESSAGE_DURATION = 2.0;

    enum RayOnAppMode {
        start,
        initialized,
        idle,
        render,
        show_sdf,
        show_normals
    };
    /*
     *
     * application class to handle useful stuff
     * like settings, user input handling etc.
     *
     * */
    class RayOnApp {
    public:
        void Init();
        void HandleUserInput();

        /*
         * proceed routines
         * draws things so has to be placed between
         * BeginDrawing();
         * &
         * EndDrawing();
         */
        void Run();
    private:
        void MakeScreenshot();
        static void EnsureScreenshotsDirExists();
        std::string GenerateScreenshotFileName();

        RayOnAppMode mode = start;

        AppSettings settings;

        /*
         * stuff to show message
         */
        std::string message_text;
        bool show_message = false;
        float message_time_left = 2.0;

        void ShowMessage(const std::string m);

        void ShowInfo();
    };
}


#endif //RAYON_CPP_CMAKE_RAYONAPP_H
