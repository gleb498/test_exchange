#pragma once

#include "exec.hpp"

#include <core/factory.hpp>

namespace exchange::api {
    class app;
}

namespace exchange::net {

    class server;

    class hub : public core::factory<hub>
    {
        friend class core::factory<hub>;

    private:
        hub(api::app& app);

    public:
        void start();
        void stop();

        exec& commands();

    private:
        std::shared_ptr<reply> on_registration(std::shared_ptr<msg_base>);
        std::shared_ptr<reply> on_order(std::shared_ptr<msg_base>);
        std::shared_ptr<reply> on_cancel_order(std::shared_ptr<msg_base>);

    protected:
        api::app&                   _app;
        std::shared_ptr<server>     _server;
        exec                        _exec;
    };
}