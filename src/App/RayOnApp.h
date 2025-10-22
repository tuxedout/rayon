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

#ifndef RAYON_CPP_CMAKE_RAYONAPP_H
#define RAYON_CPP_CMAKE_RAYONAPP_H

#include "AppSettings.h"
#include "../RayOn/Vec2.h"
#include "raylib.h"

namespace RN {
    /*
     * defaults and constants
     */
    const std::string SCREENSHOTS_FOLDER = "screenshots";
    // messages related
    const float MESSAGE_DURATION = 2.0;
    const unsigned int MESSAGE_ROW_HEIGHT = 16;
    const unsigned int MESSAGE_FONT_SIZE = 16;
    const unsigned int MESSAGE_ROWS_MAX   = 10;

    struct TextMessage{
        std::string message;
        double ttl;
        Color color;
    };

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


         // proceed routines
         // draws things so has to be placed between
         // ! BeginDrawing();
         //         &
         // ! EndDrawing();
        void Run();
        void proceedMessages();
    private:
        void MakeScreenshot();
        static void EnsureScreenshotsDirExists();

        std::string GenerateScreenshotFileName();

        // current mode
        RayOnAppMode mode = start;

        // rendering settings
        AppSettings settings;
        /*
         * stuff to show message
         */
        std::string message_text;
        bool show_message = false;

        float message_time_left = 2.0;
        void ShowMessage(const std::string &m);

        std::vector<TextMessage> messages_pool;

        void ShowInfo();
    };
}


#endif //RAYON_CPP_CMAKE_RAYONAPP_H
