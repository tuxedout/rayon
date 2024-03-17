//
// Created by root on 17.03.2024.
//

#ifndef RAYLIB_TEMPLATE_THREADPOOL_H
#define RAYLIB_TEMPLATE_THREADPOOL_H

#include <vector>
#include <thread>
#include <functional>
#include <queue>
#include <future>
#include "Segment.h"
#include <iostream>

namespace RN {

    void RenderSegment(Segment& segment, const RN::Scene& scene, RN::SFSTracer& _tracer, SDFLUT *lut, ThreadSafeQueue<Segment>& queue) {

        RN::vec2d tmp_point;

        RN::vec3 tmp_color;


        for (int y = 0; y < segment.height; y++ ) {
            for (int x = 0; x < segment.width; x++) {
                tmp_point.x = (double) (segment.x + x - segment.screen_width / 2.0)  / segment.screen_height;
                tmp_point.y = (double) (segment.y + y - segment.screen_height / 2.0) / segment.screen_height;

                //tmp_point.x *= (float)screen_width / screen_height;

                tmp_color = _tracer.sample(tmp_point, scene, lut);

                segment.data[y * segment.width + x].r = (int) (tmp_color.x * 255);
                segment.data[y * segment.width + x].g = (int) (tmp_color.y * 255);
                segment.data[y * segment.width + x].b = (int) (tmp_color.z * 255);
                segment.data[y * segment.width + x].a = 255;

                segment.end_time = std::chrono::high_resolution_clock::now();
            }
        }

        queue.push(segment);
    };

    class ThreadPool {
    public:
        ThreadPool(size_t threads, ThreadSafeQueue<Segment>& taskQueue, RN::Scene& scene, RN::SFSTracer& tracer, RN::SDFLUT* lut, ThreadSafeQueue<Segment>& resultQueue)
                : tasksQueue(taskQueue), scene(scene), tracer(tracer), lut(lut), queue(resultQueue), stop(false) {
            for (size_t i = 0; i < threads; ++i) {
                workers.emplace_back([this] {
                    // Create copies for each thread
                    RN::SFSTracer localTracer = this->tracer;
                    RN::Scene localScene = this->scene;

                    while (true) {
                        Segment segment;
                        {
                            std::unique_lock<std::mutex> lock(this->queue_mutex);
//                            this->condition.wait(lock, [this] {
//                                // Wait until stop is true or there are tasks available
//                                return this->stop || !this->tasksQueue.empty();
//                            });
                            if (this->stop && this->tasksQueue.empty())
                                break; // Exit loop if stop is true and queue is empty
                            if (!this->tasksQueue.try_pop(segment)) {
                                continue; // If nothing was popped, go back to waiting
                            }
                        }
                        // Use copies with the shared lut to render the segment
                        RenderSegment(segment, localScene, localTracer, this->lut, this->queue);
                    }
                });
            }
        }

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
        std::vector<std::thread> workers;
        ThreadSafeQueue<Segment>& tasksQueue;
        RN::Scene& scene; // Экземпляр для копирования
        RN::SFSTracer& tracer; // Экземпляр для копирования
        RN::SDFLUT* lut; // Общий указатель для чтения
        ThreadSafeQueue<Segment>& queue;
        std::mutex queue_mutex;
        std::condition_variable condition;
        bool stop;
    };
} // RN

#endif //RAYLIB_TEMPLATE_THREADPOOL_H
