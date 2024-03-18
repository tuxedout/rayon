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
 *      main-raycast.cpp - main executable file
 *
 */
#include <cstdio>
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>
#include <thread>

/*
 * 3rd party stuff
 */
#include "raylib.h"
#include "json.hpp"
using json = nlohmann::json;

#include "RayOn/Scene.h"
#include "RayOn/SFCircle.h"
#include "RayOn/SFSTracer.h"
#include "RayOn/SFBox.h"
#include "System/ThreadSafeQueue.h"
#include "RayOn/SDFLUT.h"
#include "System/ThreadPool.h"
#include "System/Segment.h"

/*
 * screen and rendering settings
 */
const int screen_width = 2000;
const int screen_height = 1000;
int segment_size = 40;
#define MAX_DISTANCE 2.50
#define SAMPLES_P_PIX 256
#define MAX_STEPS 128
#define MAX_DEPTH 10
#define EPSILON 0.0001

// TODO: move to RayOnApp
std::string GenerateFileName() {
    std::time_t now = std::time(nullptr);

    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", std::localtime(&now));

    std::string fileName = "screenshots/screenshot_" + std::string(buf) + ".png";
    return fileName;
}

ThreadSafeQueue<Segment> results_queue; // rendering results
ThreadSafeQueue<Segment> tasks_queue; // rendering tasks

RN::SDFLUT *lut; // LUT table, contains SDF data, calculates normal from distance field gradient

// split screen into small pieces and use them to create rendering tasks
std::vector<Segment> CreateAndShuffleSegments(const int screenWidth, const int screenHeight, int segmentSize) {
    std::vector<Segment> segments;

    int segmentsX = (screenWidth + segmentSize - 1) / segmentSize;
    int segmentsY = (screenHeight + segmentSize - 1) / segmentSize;

    for (int y = 0; y < segmentsY; y++) {
        for (int x = 0; x < segmentsX; x++) {
            Segment segment;
            segment.x = x * segmentSize;
            segment.y = y * segmentSize;
            segment.width = std::min(segmentSize, screenWidth - segment.x);
            segment.height = std::min(segmentSize, screenHeight - segment.y);

            // needed for render
            segment.screen_width = screenWidth;
            segment.screen_height = screenHeight;

            segment.data = new Color[segment.width * segment.height];

            segments.push_back(segment);
        }
    }

    // shuffle
    auto rng = std::default_random_engine(std::random_device{}());
    std::shuffle(std::begin(segments), std::end(segments), rng);

    return segments;
}

// TODO: move to RayOnApp
void logRenderStats(const RN::SFSTracer& tracer, long long totalDuration) {

    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);

    std::ofstream logFile("render.log", std::ios::app);

    logFile << std::put_time(std::localtime(&now_time_t), "%Y-%m-%d %H:%M:%S") << " - ";

    logFile << "Time taken: " << totalDuration << " milliseconds\n";
    logFile << "Samples per pixel: " << tracer.getSamplesPerPixel() << ", step angle: " << tracer.getStepAngle() << ", max steps: " << tracer.max_steps << ", max resursive call depth: " << tracer.max_depth << " max dist: " << tracer.max_distance << ", epsilon: " << tracer.epsilon << "\n";
    // logFile << "Stats: traces: " << tracer.getTracesCount() << ", distance: " << tracer.getDistanceMeter() << ", hits: " << tracer.getHitsCount() << ", misses: " << tracer.getMissesCount() << "\n";

    logFile.close();
}


// so, here we go
int main()
{
    // misc service variables
    bool show_message = false;
    std::string message_text;
    float message_time_left = 0.0f;
    const float message_duration = 2.0f;

    // initialization
    InitWindow(screen_width, screen_height, "RayOn");
    SetTraceLogLevel(LOG_ALL);

    Image imBlank = GenImageColor(screen_width, screen_height, WHITE); // blank image used as source for texture
    Texture2D texture = LoadTextureFromImage(imBlank);  // load blank texture to fill by renderer

    // scene setup
    // materials
    std::shared_ptr<RN::Material> mat0 = std::make_shared<RN::Material>(RN::vec3(0.87, 0.0, 1.0), RN::vec3 (0.0, 0.50, 0.0));
    std::shared_ptr<RN::Material> mat1 = std::make_shared<RN::Material>(RN::vec3(0.95, 0.95, 0.95),    RN::vec3 (0.6, 0.30, 0.20));
    std::shared_ptr<RN::Material> mat2 = std::make_shared<RN::Material>(RN::vec3(0.0,  0.0, 0.0), RN::vec3  (1.0, 0.2, 0.0));
    std::shared_ptr<RN::Material> mat3 = std::make_shared<RN::Material>(RN::vec3(0.0,  0.0, 0.0), RN::vec3 (0.9, 0.4, 0.5));
    std::shared_ptr<RN::Material> mat4 = std::make_shared<RN::Material>(RN::vec3(0.0,  0.740, 0.730), RN::vec3 (0.5, 0.0, 0.0));
    std::shared_ptr<RN::Material> mat5 = std::make_shared<RN::Material>(RN::vec3(1.50,  0.95, 0.0), RN::vec3 (0.0, 0.0, 0.0));

    mat0->reflectivity = 0.5    ;
    mat0->eta = 1.7;
    mat1->reflectivity = 0.3;
    mat1->eta = 1.3;
    mat2->reflectivity = 0.1;
    mat2->eta = 1.7;
    mat3->reflectivity = 0.1;
    mat3->eta = 1.4;
    mat4->reflectivity = 0.1;
    mat4->eta = 1.7;
    mat5->reflectivity = 0.3;
    mat5->eta = 1.97;

    // scene objects
    RN::SFCircle circle(0.1, RN::vec2(0.25,-0.30), mat0);
    RN::SFCircle circle2(0.03, RN::vec2(-0.26,0.350), mat1);
    RN::SFBox box(RN::vec2(-0.35, 0.15), RN::vec2(0.01,0.25), mat2);
    RN::SFBox box2(RN::vec2(-0.2, 0.3), RN::vec2(0.01,0.15), mat3);
    RN::SFBox box3(RN::vec2(0.1, 0.1), RN::vec2(0.1,0.15), mat4);
    RN::SFBox box4(RN::vec2(-0.02, 0.0), RN::vec2(0.01,0.01), mat5);

    RN::Scene scene(screen_width, screen_height);

    scene.addChild(std::make_shared<RN::SFCircle>(circle));
    scene.addChild(std::make_shared<RN::SFCircle>(circle2));
    scene.addChild(std::make_shared<RN::SFBox>(box));
    scene.addChild(std::make_shared<RN::SFBox>(box2));
    scene.addChild(std::make_shared<RN::SFBox>(box3));
    scene.addChild(std::make_shared<RN::SFBox>(box4));

    // SDF LUT contains screen-wide signed distance values
    // also calculates normal
    lut = new RN::SDFLUT(scene);

    // tracer-sampler-renderer, soul of machine
    RN::SFSTracer tracer;

    // set default rendering settings
    tracer.setSamplesPerPixel(SAMPLES_P_PIX);
    tracer.max_steps = MAX_STEPS;
    tracer.max_depth = MAX_DEPTH;
    tracer.max_distance = MAX_DISTANCE;
    tracer.epsilon = EPSILON;

    // get count of threads available depending on hardware configuration
    size_t threads_count = std::thread::hardware_concurrency();

    // heart of parallelism, pool of rendering threads
    // creates threads_count x threads, every thread receives own copy of scene and tracer,
    // lut used only to read from, so considered thread-safe
    // until the window is closed threads are waiting for tasks.
    // tasks placed into thread-safe queue(tasks_queue),
    // rendering results placed into another thread-safe queue(results_queue) of same type
    RN::ThreadPool pool(threads_count, tasks_queue, scene, tracer, lut, results_queue);

    // segments are rendering tasks/results, screen being split into segment_size x segment_size sized segments
    // segments being shuffled, ki krasivo olsun
    std::vector<Segment> segments = CreateAndShuffleSegments(screen_width, screen_height, segment_size);


    for (auto& segment : segments) {
        tasks_queue.push(segment); // adding segments into tasks queue
    }

    auto start = std::chrono::high_resolution_clock::now();  // Start timer
    auto end = std::chrono::high_resolution_clock::now();  // Start timer

    SetTargetFPS(30);

    // main app loop
    while (!WindowShouldClose()) {
        // TODO: move to RayOnApp
        if (show_message) {
            message_time_left -= GetFrameTime();
            if (message_time_left <= 0.0f) {
                show_message = false;
            }
        }

        Segment segment;
        Rectangle rec;
        while (results_queue.try_pop(segment)) {
            rec = { (float)segment.x, (float)segment.y, (float)segment.width, (float)segment.height };
            UpdateTextureRec(texture, rec, segment.data);

            if (segment.end_time > end){
                end = segment.end_time;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(texture, 0, 0, WHITE);
        if (show_message) {
            DrawText(message_text.c_str(), 10, 10, 20, RED);
        }
        EndDrawing();

        // TODO: move to RayOnApp
        if (IsKeyPressed(KEY_S)) {
            std::string file_name = GenerateFileName();
            TakeScreenshot(file_name.c_str());

            message_text = file_name + " saved!";
            show_message = true;
            message_time_left = message_duration;
        }

        // TODO: don't flush every frame
        fflush(stdout);
    }

    // total rendering duration
    auto total_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    printf("Time taken: %lld milliseconds\n", total_duration);

    printf("Samples per pixel: %i, step angle: %f, max steps: %i, max dist: %f, epsilon: %f\n", tracer.getSamplesPerPixel(), tracer.getStepAngle(), tracer.max_steps, tracer.max_distance, tracer.epsilon);
    //printf("Stats: traces: %i, distance: %f, hits: %i, misses: %i\n", tracer.getTracesCount(), tracer.getDistanceMeter(), tracer.getHitsCount(), tracer.getMissesCount());

    // save logs
    logRenderStats(tracer, total_duration);

    // de-initialization
    CloseWindow();

    // stop pool
    pool.stopIt();

    // free memory
    delete lut;

    UnloadTexture(texture);

    return 0;
}

// Time taken by function: 8002 milliseconds
// Samples per pixel: 16, step angle: 0.392699, max steps: 10, max dist: 15.000000, epsilon: 0.000100