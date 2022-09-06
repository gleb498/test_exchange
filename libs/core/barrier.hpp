#pragma once

#include <condition_variable>
#include <mutex>

namespace exchange::core {

    class barrier
    {
    public:
        /// create barrier to synchronize count threads
        explicit barrier(std::size_t count);

        /// wait for a thread
        void wait();

    private:
        std::size_t             _count;
        std::mutex              _mutex;
        std::condition_variable _cv;
    };
}
