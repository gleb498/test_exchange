#pragma once

#include <api_network.hpp>
#include <core/factory.hpp>

#include <boost/signals2.hpp>
#include <boost/asio.hpp>

namespace exchange::api {
    class app;
}

namespace exchange::net {

    class exec;

    class connection : public core::factory<connection>
    {
        friend class core::factory<connection>;

    private:
        connection(api::app& app, api::net::socket_t s);
    
    public:
        ~connection();

        void start();
        void stop();

        api::net::endpoint_t endpoint() const { return _endpoint; }

        boost::signals2::signal<void(std::shared_ptr<connection>)>  on_disconect_event;

    private:
        void async_read();
        void handle_read(   const boost::system::error_code& ec,
                            size_t bytes_transferred);
        void handle_write(const boost::system::error_code& ec);
        void handle_error(const boost::system::error_code& ec);

    private:
        api::app&               _app;
        exec&                   _exec;
        api::net::socket_t      _s;
        api::net::endpoint_t    _endpoint;

        enum { max_length = 1024 };
        char                    _rbuffer[max_length];
    };
}