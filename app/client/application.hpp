#pragma once

#include <api_app.hpp>
#include <core/io_threads.hpp>
#include <boost/asio/io_context.hpp>

#include <memory>

namespace exchange::net {
    class client;
}

namespace exchange::config {
    class settings;
}

namespace exchange::cli {
    class console;
}

namespace exchange {
    class application
    {
    public:
        explicit application(config::settings& cfg);
        ~application();

        application(const application&) = delete;
        application& operator=(const application&) = delete;

        void run();
        void stop();
        
        boost::asio::io_context& get_context();
        const config::settings& get_settings() const;
        net::client& get_client();

    private:
        void start();

    private:
        config::settings&               _cfg;
        boost::asio::io_context         _context;
        core::io_threads                _threads;
        std::shared_ptr<net::client>    _client;
        std::shared_ptr<cli::console>   _console;
    };
}