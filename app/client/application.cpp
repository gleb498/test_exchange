#include "application.hpp"
#include "console.hpp"

#include <network/client.hpp>

#include <spdlog/spdlog.h>
#include <boost/asio/signal_set.hpp>

namespace exchange {
    application::application(config::settings& cfg)
    :   _cfg(cfg),
        _threads(_context)
    {
        _client = net::client::create(_context, *this);
        _console = cli::console::create(_context, *this);
    }

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

    net::client& application::get_client()
    {
        return *_client;
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

        // _console->start();
        _client->connect();

        _threads.join();
    }

    void application::stop()
    {
        spdlog::info("stop client app");

        _console->stop();
        _client->stop();
        _context.stop();
    }
}