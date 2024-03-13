//
// Created by root on 13.03.2024.
//

#ifndef RAYLIB_TEMPLATE_THREADSAFEQUEUE_H
#define RAYLIB_TEMPLATE_THREADSAFEQUEUE_H


#include <mutex>
#include <queue>
#include <condition_variable>

template<typename T>
class ThreadSafeQueue {
private:
    std::queue<T> queue;
    mutable std::mutex m;
    std::condition_variable cv;
    bool isFinished = false;

public:
    ThreadSafeQueue() = default;
    ThreadSafeQueue(const ThreadSafeQueue& other) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue& other) = delete;

    void push(T value) {
        std::lock_guard<std::mutex> lock(m);
        queue.push(std::move(value));
        cv.notify_one();
    }

    bool try_pop(T& value) {
        std::lock_guard<std::mutex> lock(m);
        if (queue.empty()) {
            return false;
        }
        value = std::move(queue.front());
        queue.pop();
        return true;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(m);
        return queue.empty();
    }

    void finish() {
        std::lock_guard<std::mutex> lock(m);
        isFinished = true;
        cv.notify_all();
    }

    bool wait_and_pop(T& value) {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [this] { return !queue.empty() || isFinished; });
        if (queue.empty()) return false;
        value = std::move(queue.front());
        queue.pop();
        return true;
    }

    bool finished() const {
        std::lock_guard<std::mutex> lock(m);
        return isFinished && queue.empty();
    }
};


#endif //RAYLIB_TEMPLATE_THREADSAFEQUEUE_H
