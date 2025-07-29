#pragma once
#include <functional>
#include <future>
#include <atomic>
#include <mutex>
#include <queue>
#include <vector>
#include <thread>
#include <condition_variable>
#include <string>

#define FNTYPE void*(void)

class Threadpool {
public:
    Threadpool() : worker_count(std::thread::hardware_concurrency() ? std::thread::hardware_concurrency() * 2 : 10) { run(); }
    Threadpool(std::size_t c) : worker_count(c) { run(); }
    ~Threadpool() { shutdown(); }

    std::future<void*> spawn(std::function<FNTYPE> fn) {
        std::packaged_task<FNTYPE> task{ fn };
        auto future = task.get_future();
        {
            std::lock_guard<std::mutex> tl(tasks_lock);
            tasks.push(std::move(task));
        }
        tasks_cv.notify_one();
        return future;
    }

    void shutdown() {
        {
            // Counter intuitive lock but required for the condition_variable to be notified correctly
            // See: https://stackoverflow.com/questions/38147825/
            std::lock_guard<std::mutex> lock(tasks_lock);
            running.store(false);
        }
        tasks_cv.notify_all(); // Wake up all threads to finish up and exit
        for (auto& t : workers) {
            if (t.joinable()) {
                t.join();
            }
        }
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::packaged_task<FNTYPE>> tasks;
    std::mutex tasks_lock;
    std::condition_variable tasks_cv;
    std::atomic<bool> running{ true };
    std::size_t worker_count;

    void run() {
        for (std::size_t i = 0; i < worker_count; ++i) {
            workers.emplace_back([&]() {
                while (true) {
                    std::packaged_task<FNTYPE> task;
                    {
                        std::unique_lock<std::mutex> ul(tasks_lock);
                        tasks_cv.wait(ul, [&]() { return !tasks.empty() || !running.load(); });
                        if (!running.load() && tasks.empty()) {
                            return;
                        }
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    if (task.valid()) {
                        task();
                    }
                }
                });
        }
    }
};
#undef FNTYPE