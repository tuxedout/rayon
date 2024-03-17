#include <cstdio>
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>
#include <filesystem>
#include <fstream>
#include <thread>

#include "raylib.h"
#include "RayOn/Scene.h"
#include "RayOn/SFCircle.h"
#include "RayOn/SFSTracer.h"
#include "RayOn/SFBox.h"
#include "System/ThreadSafeQueue.h"

#include "json.hpp"
#include "RayOn/SDFLUT.h"
#include "System/ThreadPool.h"
#include "System/Segment.h"

using json = nlohmann::json;

#define MAX_DISTANCE 15.0
#define SAMPLES_P_PIX 32
#define MAX_STEPS 16
#define MAX_DEPTH 8
#define EPSILON 0.001

const int screen_width = 1000;
const int screen_height = 500;

int segment_size = 32;


void EnsureDirectoryExists(const std::string& dirName) {
    std::filesystem::path dirPath{dirName};
    if (!std::filesystem::exists(dirPath)) {
        std::filesystem::create_directories(dirPath);
    }
}

std::string GenerateFileName() {
    std::time_t now = std::time(nullptr);

    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", std::localtime(&now));

    std::string fileName = "screenshots/screenshot_" + std::string(buf) + ".png";
    return fileName;
}

ThreadSafeQueue<Segment> resultsQueue;
ThreadSafeQueue<Segment> tasksQueue;

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
            segment.screen_width = screenWidth;
            segment.screen_height = screenHeight;

            segment.data = new Color[segment.width * segment.height];

            segments.push_back(segment);
        }
    }

    auto rng = std::default_random_engine(std::random_device{}());
    std::shuffle(std::begin(segments), std::end(segments), rng);

    return segments;
}

RN::SDFLUT *lut;


void logRenderStats(const RN::SFSTracer& tracer, long long totalDuration) {
    // Получаем текущее время
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);

    // Создаем поток для записи в файл
    std::ofstream logFile("render.log", std::ios::app); // 'app' означает добавление в конец файла

    // Устанавливаем формат для времени
    logFile << std::put_time(std::localtime(&now_time_t), "%Y-%m-%d %H:%M:%S") << " - ";

    // Записываем информацию о рендеринге
    logFile << "Time taken: " << totalDuration << " milliseconds\n";
    logFile << "Samples per pixel: " << tracer.getSamplesPerPixel() << ", step angle: " << tracer.getStepAngle() << ", max steps: " << tracer.max_steps << ", max resursive call depth: " << tracer.max_depth << " max dist: " << tracer.max_distance << ", epsilon: " << tracer.epsilon << "\n";
    // logFile << "Stats: traces: " << tracer.getTracesCount() << ", distance: " << tracer.getDistanceMeter() << ", hits: " << tracer.getHitsCount() << ", misses: " << tracer.getMissesCount() << "\n";

    // Закрываем файл
    logFile.close();
}



int main()
{
    EnsureDirectoryExists("screenshots");

    // misc service variables
    bool show_message = false;
    std::string message_text;
    float message_time_left = 0.0f;
    const float message_duration = 2.0f;

    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screen_width, screen_height, "RayOn");

    SetTraceLogLevel(LOG_ALL);

    Image imBlank = GenImageColor(screen_width, screen_height, WHITE);
    Texture2D texture = LoadTextureFromImage(imBlank);  // Load blank texture to fill on shader

    std::shared_ptr<RN::Material> mat0 = std::make_shared<RN::Material>(RN::vec3(0.87, 0.0, 0.0), RN::vec3 (0.0, 0.0, 0.0));
    std::shared_ptr<RN::Material> mat1 = std::make_shared<RN::Material>(RN::vec3(0.95, 0.95, 0.95),    RN::vec3 (0.6, 0.30, 0.20));
    std::shared_ptr<RN::Material> mat2 = std::make_shared<RN::Material>(RN::vec3(0.0,  0.0, 0.0), RN::vec3  (0.04, 0.04, 0.03));
    std::shared_ptr<RN::Material> mat3 = std::make_shared<RN::Material>(RN::vec3(0.0,  0.0, 0.0), RN::vec3 (1.0, 0.0, 0.0));
    std::shared_ptr<RN::Material> mat4 = std::make_shared<RN::Material>(RN::vec3(0.0,  0.0, 0.0), RN::vec3 (0.0, 0.0, 0.0));
    std::shared_ptr<RN::Material> mat5 = std::make_shared<RN::Material>(RN::vec3(0.0,  0.65, 0.61), RN::vec3 (0.0, 0.0, 0.0));

    mat0->reflectivity = 0.20;
    mat0->eta = 0.7;
    mat1->reflectivity = 0.0;
    mat1->eta = 0.3;
    mat2->reflectivity = 0.8;
    mat2->eta = 0.7;
    mat3->reflectivity = 0.91;
    mat3->eta = 0.4;
    mat4->reflectivity = 0.45;
    mat4->eta = 0.1;
    mat5->reflectivity = 0.0;
    mat5->eta = 1.07;

     RN::SFCircle circle(0.1, RN::vec2(0.25,-0.10), mat0);
    RN::SFCircle circle2(0.03, RN::vec2(-0.26,0.350), mat1);
    RN::SFBox box(RN::vec2(-0.35, 0.15), RN::vec2(0.01,0.25), mat2);
    RN::SFBox box2(RN::vec2(-0.2, 0.3), RN::vec2(0.01,0.15), mat3);
    RN::SFBox box3(RN::vec2(-0.1, 0.1), RN::vec2(0.01,0.15), mat4);
    RN::SFBox box4(RN::vec2(-0.4, 0.0), RN::vec2(0.01,0.01), mat5);

    RN::Scene scene(screen_width, screen_height);

    scene.addChild(std::make_shared<RN::SFCircle>(circle));
    scene.addChild(std::make_shared<RN::SFCircle>(circle2));
    scene.addChild(std::make_shared<RN::SFBox>(box));
    scene.addChild(std::make_shared<RN::SFBox>(box2));
    scene.addChild(std::make_shared<RN::SFBox>(box3));
    scene.addChild(std::make_shared<RN::SFBox>(box4));

    lut = new RN::SDFLUT(scene);
    RN::SFSTracer tracer;

    tracer.setSamplesPerPixel(SAMPLES_P_PIX);
    tracer.max_steps = MAX_STEPS;
    tracer.max_depth = MAX_DEPTH;
    tracer.max_distance = MAX_DISTANCE;
    tracer.epsilon = EPSILON;

    size_t threadsCount = std::thread::hardware_concurrency();
    RN::ThreadPool pool(threadsCount, tasksQueue, scene, tracer, lut, resultsQueue);

    std::vector<Segment> segments = CreateAndShuffleSegments(screen_width, screen_height, segment_size);

    for (auto& segment : segments) {
        tasksQueue.push(segment); // Добавляем задачи в очередь
    }

    auto start = std::chrono::high_resolution_clock::now();  // Start timer
    auto end = std::chrono::high_resolution_clock::now();  // Start timer

    SetTargetFPS(30);
    while (!WindowShouldClose()) {
        if (show_message) {
            message_time_left -= GetFrameTime();
            if (message_time_left <= 0.0f) {
                show_message = false;
            }
        }

        Segment segment;
        Rectangle rec;
        while (resultsQueue.try_pop(segment)) {
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

        if (IsKeyPressed(KEY_S)) {
            std::string file_name = GenerateFileName();
            TakeScreenshot(file_name.c_str());

            message_text = file_name + " saved!";
            show_message = true;
            message_time_left = message_duration;
        }

        fflush(stdout);
    }

//    auto start = std::chrono::high_resolution_clock::now();  // Start timer
//    auto stop = std::chrono::high_resolution_clock::now();  // Stop timer
//    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
//    printf("Time taken by function: %lld milliseconds\n", duration.count());

    auto totalDuration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    printf("Time taken: %lld milliseconds\n", totalDuration);

    printf("Samples per pixel: %i, step angle: %f, max steps: %i, max dist: %f, epsilon: %f\n", tracer.getSamplesPerPixel(), tracer.getStepAngle(), tracer.max_steps, tracer.max_distance, tracer.epsilon);
    //printf("Stats: traces: %i, distance: %f, hits: %i, misses: %i\n", tracer.getTracesCount(), tracer.getDistanceMeter(), tracer.getHitsCount(), tracer.getMissesCount());

    logRenderStats(tracer, totalDuration);

    // De-Initialization
    CloseWindow();

    pool.stopIt();

    delete lut;

    UnloadTexture(texture);

    return 0;
}

// Time taken by function: 8002 milliseconds
// Samples per pixel: 16, step angle: 0.392699, max steps: 10, max dist: 15.000000, epsilon: 0.000100