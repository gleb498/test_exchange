#include "io_threads.hpp"
#include "log.hpp"
#include "barrier.hpp"

#include <boost/asio.hpp>

namespace exchange::core {
    io_threads::io_threads(boost::asio::io_context& ctx)
    :   _ctx(ctx)
    {}

    io_threads::~io_threads()
    {
        join();
    }

    std::size_t io_threads::start(std::size_t n)
    {
        _threads.reserve(n);

        std::shared_ptr<barrier> b = std::make_shared<barrier>(n+1);
        for(std::size_t i = 0; i < n; ++i)
        {
            std::thread t([this, b]() {
                b->wait();
                run();
            });
            _threads.emplace_back(std::move(t));
        }
        b->wait();
        return n;
    }

    void io_threads::join()
    {
        for(auto& t : _threads)
            if(t.joinable())
                t.join();
        _threads.clear();
    }

    void io_threads::run()
    {
        while(!_ctx.stopped())
        {
            try
            {
                _ctx.run();
            }
            catch(const std::exception& e)
            {
                spdlog::error(e.what());
            }
        }
    }
}