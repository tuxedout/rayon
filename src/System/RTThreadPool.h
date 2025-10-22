/*
 *
 *       Created by tux on 17.03.2024.
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
 *      RTThreadPool - pool of rendering threads, manages multithreading things
 *
 *
 *
 *      heart of parallelism, pool of rendering threads
 *      creates requested count of threads,
 *      every thread receives own copy of scene and sampler,
 *
 *      lut used only to read from, so considered thread-safe
 *
 *      until the window is closed threads are waiting for tasks.
 *      tasks placed into thread-safe queue(tasks_queue),
 *      rendering results placed into another thread-safe queue(results_queue) of same type
 *      task and rendering results managed by Segment class
 */

#ifndef RAYON_RTThreadPool_H
#define RAYON_RTThreadPool_H

#include <utility>
#include <vector>
#include <thread>
#include <functional>
#include <queue>
#include <future>
#include <iostream>

#include "Segment.h"
#include "../RayOn/plus-d/Containers/RTScene.h"
#include "../RayOn/plus-d/Samplers/RTSampler.h"
#include "ThreadSafeQueue.h"
#include "../RayOn/plus-d/Containers/RTLightsList.h"

namespace RN {
    // execute rendering task and save results
    void RTRenderSegment(Segment& segment, std::shared_ptr<RN::RTScene> &scene, std::shared_ptr<RN::RTLightsList> &lights, RN::RTSampler &_tracer, ThreadSafeQueue<Segment>& results_queue) {

        RN::vec2i tmp_point;

        RN::vec3d tmp_color;

        for (int y = 0; y < segment.height; y++ ) {
            for (int x = 0; x < segment.width; x++) {
                // render point
                tmp_point.x = x + segment.x;
                tmp_point.y = y + segment.y;

                tmp_color = _tracer.sampleAAS(tmp_point, scene, lights);

                // save data in format suitable for texture
                // TODO: use non linear color space
                segment.data[y * segment.width + x].r = (int) (tmp_color.x * 255);
                segment.data[y * segment.width + x].g = (int) (tmp_color.y * 255);
                segment.data[y * segment.width + x].b = (int) (tmp_color.z * 255);
                segment.data[y * segment.width + x].a = 255;
            }
        }

        // save end_time
        segment.end_time = std::chrono::high_resolution_clock::now();

        // add results to results
        results_queue.push(segment);
    };

    class RTThreadPool {
    public:
        RTThreadPool(size_t threads, ThreadSafeQueue<Segment>& taskQueue, std::shared_ptr<RN::RTScene> scene, std::shared_ptr<RN::RTLightsList> lights, std::shared_ptr<RN::RTSampler> sampler, ThreadSafeQueue<Segment>& resultQueue):
                threads(threads), tasksQueue(taskQueue), scene(std::move(scene)), lights(lights), sampler(*sampler), queue(resultQueue), stop(false) {

            restart();
        }

        void restart() {
            workers.clear();

            for (size_t i = 0; i < threads; ++i) {
                workers.emplace_back([this] {
                    // Create copies for each thread
                    RN::RTSampler localTracer = this->sampler;
                    std::shared_ptr<RN::RTScene> localScene = this->scene;
                    std::shared_ptr<RN::RTLightsList> localLights = this->lights;

                    while (true) {
                        Segment segment;
                        {
                            //std::unique_lock<std::mutex> lock(this->queue_mutex);
//                            this->condition.wait(lock, [this] {
//                                // Wait until stop is true or there are tasks available
//                                return this->stop || !this->tasks_queue.empty();
//                            });
                            if (this->stop)
                                break; // Exit loop if stop is true and queue is empty
                            if (!this->tasksQueue.try_pop(segment)) {
                                continue; // If nothing was popped, go back to waiting
                            }
                        }
                        // sse copies of scene and sampler with the shared lut to render the segment
                        RTRenderSegment(segment, localScene, localLights, localTracer, this->queue);
                    }
                });
            }
        }

        // stop pool and threads
        void stopIt(){
            stop = true;
        }

        ~RTThreadPool() {
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                stop = true;
            }
            condition.notify_all();
            for(std::thread &worker: workers)
                worker.join();
        }
    private:
        // TODO: add _!

        // threads
        std::vector<std::thread> workers;


        std::shared_ptr<RN::RTScene> scene;
        std::shared_ptr<RN::RTLightsList> lights;
        RTSampler sampler;

        ThreadSafeQueue<Segment> &tasksQueue;
        ThreadSafeQueue<Segment> &queue;

        std::mutex queue_mutex;
        std::condition_variable condition;

        size_t threads = 1;

        // is pool stopped?
        bool stop;
    };
} // RN

#endif //RAYON_RTThreadPool_H
