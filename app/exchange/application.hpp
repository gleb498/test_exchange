#pragma once

#include <api_app.hpp>
#include <core/io_threads.hpp>
#include <boost/asio/io_context.hpp>

#include <memory>

namespace exchange::net {
    class hub;
}

namespace exchange::config {
    class settings;
}

namespace exchange {
    class application : public api::app
    {
    public:
        explicit application(config::settings& cfg);
        ~application();

        application(const application&) = delete;
        application& operator=(const application&) = delete;

        void run() override;
        void stop() override;
        
        boost::asio::io_context&    get_context() override;
        const config::settings&     get_settings() const override;
        net::hub&                   get_hub() override;

    private:
        void start();

    private:
        config::settings&               _cfg;
        boost::asio::io_context         _context;
        core::io_threads                _threads;
        std::shared_ptr<net::hub>       _hub;
    };
}