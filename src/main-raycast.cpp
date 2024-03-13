#include <cstdio>
#include <chrono>
#include <memory>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <filesystem>

#include "raylib.h"
#include "RayOn/Scene.h"
#include "RayOn/SFCircle.h"
#include "RayOn/SFSTracer.h"
#include "RayOn/SFBox.h"
#include "System/ThreadSafeQueue.h"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif

#define MAX_DISTANCE 15.0
#define SAMPLES_P_PIX 64
#define MAX_STEPS 20
#define MAX_DEPTH 5
#define EPSILON 0.0001

const int screen_width = 2000;
const int screen_height = 1000;

int segment_size = 20;

std::atomic<std::chrono::time_point<std::chrono::high_resolution_clock>> lastFinished;

void EnsureDirectoryExists(const std::string& dirName) {
    std::filesystem::path dirPath{dirName};
    if (!std::filesystem::exists(dirPath)) {
        std::filesystem::create_directories(dirPath);
    }
}

struct Segment {
    int x, y, width, height;
    Color* data;

    Segment() : x(0), y(0), width(0), height(0), data(nullptr) {}

    Segment(const Segment& other)
            : x(other.x), y(other.y), width(other.width), height(other.height), data(nullptr) {
        if (other.data) {
            data = new Color[width * height];
            std::copy(other.data, other.data + width * height, data);
        }
    }

    Segment& operator=(const Segment& other) {
        if (this != &other) {

            delete[] data;

            x = other.x;
            y = other.y;
            width = other.width;
            height = other.height;
            data = new Color[width * height];
            std::copy(other.data, other.data + width * height, data);
        }
        return *this;
    }

    ~Segment() {
        delete[] data;
    }
};


std::vector<Segment> createAndShuffleSegments(int screenWidth, int screenHeight, int segmentSize) {
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

            segment.data = new Color[segment.width * segment.height];

            segments.push_back(segment);
        }
    }

    auto rng = std::default_random_engine(std::random_device{}());
    std::shuffle(std::begin(segments), std::end(segments), rng);

    return segments;
}

void renderSegment(Segment& segment, const RN::Scene& scene, RN::SFSTracer& tracer, ThreadSafeQueue<Segment>& queue) {
    glm::vec2 tmp_point(0.0, 0.0);

    glm::vec3 tmp_color(0.0,0.0,0.0);

    Color clr;

    for (int y = 0; y < segment.height; y++ ) {
        for (int x = 0; x < segment.width; x++) {
            tmp_point.x = (float) (segment.x + x - screen_width / 2.0) / screen_width;
            tmp_point.y = (float) (segment.y + y - screen_height / 2.0) / screen_height;


            tmp_point.x *= (float)screen_width / screen_height;

            tmp_color = tracer.sample(tmp_point, scene);

            clr.r = (int) (tmp_color.x * 255);
            clr.g = (int) (tmp_color.y * 255);
            clr.b = (int) (tmp_color.z * 255);
            clr.a = 255;

            segment.data[y * segment.width + x] = clr;
        }
    }

    queue.push(segment);

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::time_point<std::chrono::high_resolution_clock> expected = lastFinished.load();

    while (!lastFinished.compare_exchange_strong(expected, endTime) && expected < endTime) {
        expected = lastFinished.load();
    }
}

ThreadSafeQueue<Segment> queue;

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screen_width, screen_height, "RayOn");

    SetTraceLogLevel(LOG_ALL);

    Image imBlank = GenImageColor(screen_width, screen_height, WHITE);
    Texture2D texture = LoadTextureFromImage(imBlank);  // Load blank texture to fill on shader

    std::shared_ptr<RN::Material> mat0 = std::make_shared<RN::Material>(glm::vec3(0.40, 0.0, 0.00), glm::vec3 (0.0, 0.0, 0.0));
    std::shared_ptr<RN::Material> mat1 = std::make_shared<RN::Material>(glm::vec3(0.3, 0.3, 0.2), glm::vec3 (0.0, 0.30, 0.20));
    std::shared_ptr<RN::Material> mat2 = std::make_shared<RN::Material>(glm::vec3(0.150,  0.150,  0.150), glm::vec3  (0.04, 0.04, 0.03));
    std::shared_ptr<RN::Material> mat3 = std::make_shared<RN::Material>(glm::vec3(0.10,  0.10, 0.10), glm::vec3 (1.0, 0.0, 0.0));

    mat0->reflectivity = 0.910;
    mat0->eta = 0.0;
    mat1->reflectivity = 1.0;
    mat1->eta = 0.3;
    mat2->reflectivity = 0.4;
    mat2->eta = 1.4;
    mat3->reflectivity = 0.51;
    mat3->eta = 0.3;

    RN::SFCircle circle(0.1, glm::vec2(0.25,-0.10), mat0);
    RN::SFCircle circle2(0.03, glm::vec2(-0.1,0.0), mat1);
    RN::SFBox box(glm::vec2(-0.35, 0.15), glm::vec2(0.01,0.25), mat2);
    RN::SFBox box2(glm::vec2(-0.2, 0.35), glm::vec2(0.01,0.1), mat3);

    RN::Scene scene;

    scene.addChild(std::make_shared<RN::SFCircle>(circle));
    scene.addChild(std::make_shared<RN::SFCircle>(circle2));
    //scene.addChild(std::make_shared<RN::SFBox>(box));
    scene.addChild(std::make_shared<RN::SFBox>(box2));

    RN::SFSTracer tracer;

    tracer.setSamplesPerPixel(SAMPLES_P_PIX);
    tracer.max_steps = MAX_STEPS;
    tracer.max_depth = MAX_DEPTH;
    tracer.max_distance = MAX_DISTANCE;
    tracer.epsilon = EPSILON;

    std::vector<Segment> segments = createAndShuffleSegments(screen_width, screen_height, segment_size);
    std::vector<std::thread> threads;

    lastFinished = std::chrono::high_resolution_clock::now();

    auto start = std::chrono::high_resolution_clock::now();  // Start timer

    threads.reserve(segments.size());
    for (auto& segment : segments) {
        threads.emplace_back(renderSegment, std::ref(segment), std::cref(scene), std::ref(tracer), std::ref(queue));
    }

    SetTargetFPS(30);
    while (!WindowShouldClose()) {
        Segment segment;
        Rectangle rec;
        while (queue.try_pop(segment)) {
            rec = { (float)segment.x, (float)segment.y, (float)segment.width, (float)segment.height };
            UpdateTextureRec(texture, rec, segment.data);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(texture, 0, 0, WHITE);
        EndDrawing();
    }

    for (auto& thread : threads) {
        thread.join();
    }

    queue.finish();

//    auto start = std::chrono::high_resolution_clock::now();  // Start timer
//    auto stop = std::chrono::high_resolution_clock::now();  // Stop timer
//    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
//    printf("Time taken by function: %lld milliseconds\n", duration.count());

    auto totalDuration = std::chrono::duration_cast<std::chrono::milliseconds>(lastFinished.load() - start).count();

    printf("Time taken by function: %lld milliseconds\n", totalDuration);

    printf("Samples per pixel: %i, step angle: %f, max steps: %i, max dist: %f, epsilon: %f\n", tracer.getSamplesPerPixel(), tracer.getStepAngle(), tracer.max_steps, tracer.max_distance, tracer.epsilon);
    printf("Stats: traces: %i, distance: %f, hits: %i, misses: %i\n", tracer.getTracesCount(), tracer.getDistanceMeter(), tracer.getHitsCount(), tracer.getMissesCount());

    // De-Initialization
    CloseWindow();

    UnloadTexture(texture);

    return 0;
}
