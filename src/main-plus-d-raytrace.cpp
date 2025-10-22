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
 *      main-plus-d-raytrace.cpp
 *
 */
#include <cstdio>
#include <vector>
#include <algorithm>
#include <random>
#include <thread>

/*
 * 3rd party stuff
 */
#include "raylib.h"
#include "json.hpp"

#include "Misc/DiscreteInterval.h"
#include "Misc/Grid.h"

#include "App/RayOnApp.h"
#include "System/ThreadSafeQueue.h"
#include "System/Segment.h"
#include "System/RTThreadPool.h"
#include "RayOn/Materials/ReflectingMaterial.h"
#include "RayOn/plus-d/Containers/RTScene.h"
#include "RayOn/plus-d/Samplers/RTSampler.h"
#include "RayOn/plus-d/Primitives/RTSphere.h"
#include "RayOn/plus-d/BVH.h"
#include "RayOn/Textures/CheckeredTexture.h"
#include "RayOn/Textures/ImageTexture.h"
#include "RayOn/Materials/RefractingMaterial.h"
#include "RayOn/Textures/LinesTexture.h"
#include "RayOn/plus-d/Primitives/RTBox.h"
#include "RayOn/Materials/DiffusiveMaterial.h"
#include "RayOn/Textures/CheckeredColorTexture.h"
#include "RayOn/plus-d/Primitives/RTQuad.h"
#include "Scenes/MainScene.h"
#include "Scenes/OpticalTest.h"
#include "Scenes/OpticalTest2.h"
#include "Scenes/TextureTest.h"
#include "Scenes/ObjTest.h"
#include "Scenes/SCHUM.h"
#include "Scenes/SCHUM-BW.h"
#include "Scenes/Shapes.h"
#include "Scenes/RayTraceTest.h"
#include "Scenes/RotateTranslate.h"
#include "Scenes/GlassScene.h"


using json = nlohmann::json;

/*
 * screen and rendering settings
 */
const int screen_width = 2300;
const int screen_height = 1300;
int segment_size = 256;



ThreadSafeQueue<Segment> results_queue; // rendering results
ThreadSafeQueue<Segment> tasks_queue; // rendering tasks

RN::RayOnApp app;


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

// so, here we go
int main() {
    // initialization
    InitWindow(screen_width, screen_height, "RayOn");
    SetTraceLogLevel(LOG_ALL);

    Image imBlank = GenImageColor(screen_width, screen_height, WHITE); // blank image used as source for texture
    Texture2D texture = LoadTextureFromImage(imBlank);  // load blank texture to fill by renderer

    RN::Camera camera(1.0, screen_width, screen_height, RN::vec3d(), 0*RN::PI_D/180);


    RN::RTScene scene;

    RN::OpticalTest2 main_s;

    main_s.initScene(scene, <#initializer#>, camera);

    RN::RTSampler sampler(camera);

    //RN::BVH bvh_scene = RN::BVH(scene.children(), 0, scene.children().size());
    RN::BVH bvh_scene = RN::BVH(scene);

    // get count of threads available depending on hardware configuration
    size_t threads_count = std::thread::hardware_concurrency();
//    size_t threads_count = 1;

    // segments are rendering tasks/results, screen being split into segment_size x segment_size sized segments
    // segments being shuffled, ki krasivo olsun
    std::vector<Segment> segments = CreateAndShuffleSegments(screen_width, screen_height, segment_size);

    // heart of parallelism, pool of rendering threads
    // creates threads_count x threads, every thread receives own copy of scene and sampler,
    // lut used only to read from, so considered thread-safe
    // until the window is closed threads are waiting for tasks.
    // tasks placed into thread-safe queue(tasks_queue),
    // rendering results placed into another thread-safe queue(results_queue) of same type
    RN::RTThreadPool pool(threads_count, tasks_queue, std::make_shared<RN::BVH>(bvh_scene), sampler, results_queue);
    //RN::RTThreadPool pool(threads_count, tasks_queue, std::make_shared<RN::RTScene>(scene), sampler, results_queue);

    auto start = std::chrono::high_resolution_clock::now();  // start timer
    auto end = std::chrono::high_resolution_clock::now();  // end timer

    for (auto& segment : segments) {
        tasks_queue.push(segment); // adding segments into tasks queue
    }

    SetTargetFPS(60);

    unsigned long int frame_counter;

    long long int total_duration;

    // main app loop
    while (!WindowShouldClose()) {
        Segment segment;
        Rectangle rec;


        while (results_queue.try_pop(segment)) {
            rec = { (float)segment.x, (float)segment.y, (float)segment.width, (float)segment.height };
            UpdateTextureRec(texture, rec, segment.data);

            if (segment.end_time > end){
                end = segment.end_time;

                total_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(texture, 0, 0, WHITE);
        app.Run();
        EndDrawing();

        app.HandleUserInput();

        // TODO: don't flush every frame
        if (frame_counter % 100) {
            printf("Time taken: %lld milliseconds\n", total_duration);
            fflush(stdout);
        }
    }

    printf("Time taken: %lld milliseconds\n", total_duration);

    // de-initialization
    CloseWindow();

    UnloadTexture(texture);

    return 0;
}