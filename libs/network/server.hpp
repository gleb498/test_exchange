#pragma once

#include <core/factory.hpp>
#include <api_network.hpp>

namespace exchange::api {
    class app;
}

namespace exchange::net {
    class server : public core::factory<server>
    {
        friend class core::factory<server>;

    private:
        server(api::app& app);

    public:
        void start();
        void stop();

    protected:
        void start_accept();
        void handle_accept(api::net::socket_t&& s);
        void handle_error(const boost::system::error_code& error);
        
    protected:
        api::app&                   _app;
        api::net::endpoint_t        _local;
        api::net::acceptor_t        _s;
    };
}