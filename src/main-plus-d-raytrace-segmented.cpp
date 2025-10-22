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
#include "Scenes/CORNEIL.h"
#include "Scenes/CORNEIL-BENCHY.h"
#include "RayOn/plus-d/Containers/RTLightsList.h"
#include "Scenes/SCHUM-BW-REFLECT.h"


using json = nlohmann::json;

/*
 * screen and rendering settings
 */
const int screen_width = 1024 + 256;
const int screen_height = 1024 + 256;
int segment_size_w = 1;
int segment_size_h = 1;



ThreadSafeQueue<Segment> results_queue; // rendering results
ThreadSafeQueue<Segment> tasks_queue; // rendering tasks

RN::RayOnApp app;


// split screen into small pieces and use them to create rendering tasks
std::vector<Segment> CreateAndShuffleSegments(const int screenWidth, const int screenHeight, int segmentSizeW, int segmentSizeH) {
    std::vector<Segment> segments;

    int segmentsX = (screenWidth + segmentSizeW - 1) / segmentSizeW;
    int segmentsY = (screenHeight + segmentSizeH - 1) / segmentSizeH;

    for (int y = 0; y < segmentsY; y++) {
        for (int x = 0; x < segmentsX; x++) {
            Segment segment;
            segment.x = x * segmentSizeW;
            segment.y = y * segmentSizeH;
            segment.width = std::min(segmentSizeW, screenWidth - segment.x);
            segment.height = std::min(segmentSizeH, screenHeight - segment.y);

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

    RN::Camera camera(0.1, screen_width, screen_height, RN::vec3d(), 0*RN::PI_D/180);

    RN::RTScene scene;
    RN::RTLightsList lights;

    RN::CorneilBenchyScene main_s;

    main_s.initScene(scene, lights, camera);

    lights.pdf.setItems(lights.children());

    RN::RTSampler sampler(camera, lights);

    //RN::BVH bvh_scene = RN::BVH(scene.children(), 0, scene.children().size());
    RN::BVH bvh_scene = RN::BVH(scene);

    // get count of threads available depending on hardware configuration
    size_t threads_count = std::thread::hardware_concurrency();
//    size_t threads_count = 1;

    // segments are rendering tasks/results, screen being split into segment_size x segment_size sized segments
    // segments being shuffled, ki krasivo olsun
    std::vector<Segment> segments = CreateAndShuffleSegments(screen_width, screen_height, segment_size_w, segment_size_h);

    // heart of parallelism, pool of rendering threads
    // creates threads_count x threads, every thread receives own copy of scene and sampler,
    // lut used only to read from, so considered thread-safe
    // until the window is closed threads are waiting for tasks.
    // tasks placed into thread-safe queue(tasks_queue),
    // rendering results placed into another thread-safe queue(results_queue) of same type
    RN::RTThreadPool pool(threads_count, tasks_queue, std::make_shared<RN::BVH>(bvh_scene), std::make_shared<RN::RTLightsList>(lights), std::make_shared<RN::RTSampler>(sampler), results_queue);
    //RN::RTThreadPool pool(threads_count, tasks_queue, std::make_shared<RN::RTScene>(scene), sampler, results_queue);

    auto start = std::chrono::high_resolution_clock::now();  // start timer
    auto end = std::chrono::high_resolution_clock::now();  // end timer

    for (auto& segment : segments) {
        tasks_queue.push(segment); // adding segments into tasks queue
    }

    SetTargetFPS(60);

    unsigned long int frame_counter = 0;

    long long int total_duration = 0;

    // main app loop
    while (!WindowShouldClose()) {
        Segment segment;
        Rectangle rec;

        if ((frame_counter % 4) == 0) {
            while (results_queue.try_pop(segment)) {
                rec = {(float) segment.x, (float) segment.y, (float) segment.width, (float) segment.height};
                UpdateTextureRec(texture, rec, segment.data);

                if (segment.end_time > end) {
                    end = segment.end_time;

                    total_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(texture, 0, 0, WHITE);
        app.Run();
        EndDrawing();

        app.HandleUserInput();

        // TODO: don't flush every frame
        if ((frame_counter % 100) == 0 && total_duration != 0) {
            printf("Time taken: %lld milliseconds\n", total_duration);
            fflush(stdout);
        }

        frame_counter++;
    }

    printf("Time taken: %lld milliseconds\n", total_duration);

    // de-initialization
    CloseWindow();

    UnloadTexture(texture);

    return 0;
}