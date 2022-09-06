#pragma once

#include <thread>
#include <vector>

namespace boost::asio {
    class io_context;
}

namespace exchange::core {

    constexpr const uint32_t MINIMUM_REQUIRED_THREADS = 4;
    
    class io_threads
    {
    public:
        explicit io_threads(boost::asio::io_context& ctx);
        ~io_threads();

        std::size_t start(std::size_t n = MINIMUM_REQUIRED_THREADS);
        void join();

    private:
        boost::asio::io_context& _ctx;
        std::vector<std::thread> _threads;

        void run();
    };
}
