#include "application.hpp"

#include <network/hub.hpp>

#include <spdlog/spdlog.h>
#include <boost/asio/signal_set.hpp>

namespace exchange {
    application::application(config::settings& cfg)
    :   _cfg(cfg),
        _threads(_context),
        _hub(net::hub::create(*this))
    {}

    application::~application()
    {
        stop();
    }

    void application::run()
    {
        start();
    }

    boost::asio::io_context& application::get_context()
    {
        return _context;
    }

    const config::settings& application::get_settings() const
    {
        return _cfg;
    }

    net::hub& application::get_hub()
    {
        return *_hub;
    }

    void application::start()
    {
        boost::asio::signal_set signals(_context, SIGINT, SIGTERM);
        signals.async_wait([this](auto error, auto sig)
        {
            if (!error) 
                stop(); 
        });

        std::size_t n = _threads.start();
        spdlog::info("started {} threads in main thread pool", n);

        _hub->start();

        _threads.join();
    }

    void application::stop()
    {
        spdlog::info("stop exchange app");
        
        _hub->stop();
        _context.stop();
    }
}