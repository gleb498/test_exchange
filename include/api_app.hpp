#pragma once

namespace boost::asio {
    class io_context;
}

namespace exchange::config {
    class settings;
}

namespace exchange::api {
    class app
    {
    public:
        virtual ~app() {};
        virtual void run() = 0;
        virtual void stop() = 0;
        virtual boost::asio::io_context& get_context() = 0;
        virtual const config::settings& get_settings() const = 0;
    };
}
