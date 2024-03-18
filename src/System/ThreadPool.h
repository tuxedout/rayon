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
 *      ThreadPool - pool of rendering threads, manages multithreading things
 *
 *
 *
 *      heart of parallelism, pool of rendering threads
 *      creates requested count of threads,
 *      every thread receives own copy of scene and tracer,
 *
 *      lut used only to read from, so considered thread-safe
 *
 *      until the window is closed threads are waiting for tasks.
 *      tasks placed into thread-safe queue(tasks_queue),
 *      rendering results placed into another thread-safe queue(results_queue) of same type
 *      task and rendering results managed by Segment class
 */

#ifndef RAYLIB_TEMPLATE_THREADPOOL_H
#define RAYLIB_TEMPLATE_THREADPOOL_H

#include <vector>
#include <thread>
#include <functional>
#include <queue>
#include <future>
#include <iostream>

#include "Segment.h"

namespace RN {
    // execute rendering task and save results
    void RenderSegment(Segment& segment, const RN::Scene& scene, RN::SFSTracer& _tracer, SDFLUT *lut, ThreadSafeQueue<Segment>& results_queue) {

        RN::vec2d tmp_point;

        RN::vec3 tmp_color;

        for (int y = 0; y < segment.height; y++ ) {
            for (int x = 0; x < segment.width; x++) {
                // get scene-space coordinates
                tmp_point.x = (double) (segment.x + x - segment.screen_width / 2.0)  / segment.screen_height;
                tmp_point.y = (double) (segment.y + y - segment.screen_height / 2.0) / segment.screen_height;

                // render point
                tmp_color = _tracer.sample(tmp_point, scene, lut);

                // save data in format suitable for texture
                segment.data[y * segment.width + x].r = (int) (tmp_color.x * 255);
                segment.data[y * segment.width + x].g = (int) (tmp_color.y * 255);
                segment.data[y * segment.width + x].b = (int) (tmp_color.z * 255);
                segment.data[y * segment.width + x].a = 255;

                // save end_time
                segment.end_time = std::chrono::high_resolution_clock::now();
            }
        }

        // add results to results
        results_queue.push(segment);
    };

    class ThreadPool {
    public:
        ThreadPool(size_t threads, ThreadSafeQueue<Segment>& taskQueue, RN::Scene& scene, RN::SFSTracer& tracer, RN::SDFLUT* lut, ThreadSafeQueue<Segment>& resultQueue):
        tasksQueue(taskQueue), scene(scene), tracer(tracer), lut(lut), queue(resultQueue), stop(false) {
            for (size_t i = 0; i < threads; ++i) {
                workers.emplace_back([this] {
                    // Create copies for each thread
                    RN::SFSTracer localTracer = this->tracer;
                    RN::Scene localScene = this->scene;

                    while (true) {
                        Segment segment;
                        {
                            //std::unique_lock<std::mutex> lock(this->queue_mutex);
//                            this->condition.wait(lock, [this] {
//                                // Wait until stop is true or there are tasks available
//                                return this->stop || !this->tasks_queue.empty();
//                            });
                            if (this->stop && this->tasksQueue.empty())
                                break; // Exit loop if stop is true and queue is empty
                            if (!this->tasksQueue.try_pop(segment)) {
                                continue; // If nothing was popped, go back to waiting
                            }
                        }
                        // sse copies of scene and tracer with the shared lut to render the segment
                        RenderSegment(segment, localScene, localTracer, this->lut, this->queue);
                    }
                });
            }
        }

        // stop pool and threads
        void stopIt(){
            stop = true;
        }

        ~ThreadPool() {
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                stop = true;
            }
            condition.notify_all();
            for(std::thread &worker: workers)
                worker.join();
        }
    private:
        // threads
        std::vector<std::thread> workers;


        RN::Scene& scene;
        RN::SFSTracer& tracer;

        // shared, used only to read data from
        RN::SDFLUT* lut;

        ThreadSafeQueue<Segment>& tasksQueue;
        ThreadSafeQueue<Segment>& queue;

        std::mutex queue_mutex;
        std::condition_variable condition;

        // is pool stopped?
        bool stop;
    };
} // RN

#endif //RAYLIB_TEMPLATE_THREADPOOL_H
