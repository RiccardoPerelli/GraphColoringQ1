#ifndef GRAPHCOLORINGPERELLI_PRODCONSVECTOR_H
#define GRAPHCOLORINGPERELLI_PRODCONSVECTOR_H

#include <functional>
#include <mutex>
#include <optional>
#include <thread>
#include <vector>
#include <semaphore.h>
#include <iostream>

#if __linux__
#include <pthread.h>
#endif

// A vector that implements the producer-consumer pattern.
template <typename T>
class ProdConsV {
private:
    std::vector<T> data;
    bool stopped;

    std::mutex mutex;
    sem_t full_or_done;

    // Threads used by onReceive
    std::vector<std::thread> threads;

public:

    //constructor with no data
    ProdConsV() : stopped(false) {
        sem_init(&full_or_done, 0, 0);
    }

    //constructor with data
    ProdConsV(std::vector<T> &&_data) : data(std::move(_data)), stopped(false) {
        sem_init(&full_or_done, 0, data.size());
    }

    ~ProdConsV() {
        sem_destroy(&full_or_done);
    }

    // utilizzo il mutex per aggiungere un elemento alla lista dei dati consumabili
    // una volta fatto questo faccio un sem post per notificare a tutti il nuovo item
    void push(const T &val) {
        if (stopped)
            throw std::runtime_error("Writing to stopped queue");
        mutex.lock();
        data.emplace_back(val);
        mutex.unlock();
        sem_post(&full_or_done);
    };

    // Consumo un oggetto se non c'è nulla aspetto che ci sia
    std::optional<T> pop() {
        sem_wait(&full_or_done);
        if (stopped) {
            sem_post(&full_or_done);
        }
        std::unique_lock lock(mutex);
        if (done())
            return std::nullopt;
        T ret = data.back();
        data.pop_back();
        return ret;
    };

    void onReceive(int num_threads, void (*callback)(T)) {
        for (int i = 0; i < num_threads; i++)
            threads.emplace_back([](ProdConsV<T> &queue, void (*callback)(T), int i) {
                while (std::optional<T> item = queue.pop()) {
                                         callback(*item);
                                     }
                                 },
                                 std::ref(*this), callback, i);
    }

    void onReceive(int num_threads, std::function<void(T)> callback) {
        for (int i = 0; i < num_threads; i++)
            threads.emplace_back([=](ProdConsV<T> &queue, int i) {
                                     while (std::optional<T> item = queue.pop()) {
                                         callback(*item);
                                     }
                                 },
                                 std::ref(*this), i);
    }

    // Signal that there are no more items to be produced
    // Do a "fictitious" post on full_or_done so it can wake up threads waiting for an element
    void stop() {
        stopped = true;
        sem_post(&full_or_done);
    };

    // Join any onReceive thread
    void join() {
        for (auto &t : threads)
            t.join();
    }

    bool done() const {
        return stopped && data.empty();
    };

};

#endif //GRAPHCOLORINGPERELLI_PRODCONSVECTOR_H
