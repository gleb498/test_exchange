#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/streambuf.hpp>
#include <api_network.hpp>

#include <core/factory.hpp>
#include <atomic>

namespace exchange::api {
    class app;
}

namespace exchange::net {

    class client : public core::factory<client>
    {
        friend class core::factory<client>;

    private:
        client(api::app& app);

    public:
        void connect();
        void call(const std::string& msg);
        void stop();

    private:
        void start_connect(api::net::tcp::resolver::iterator endpoint_iter);
        void handle_connect(const boost::system::error_code& ec,
                            api::net::tcp::resolver::iterator endpoint_iter);

        void start_read();
        void handle_read(const boost::system::error_code& ec);

        void handle_write(const boost::system::error_code& ec);

        void handle_error(const boost::system::error_code& ec);
        
    private:
        api::app&                   _app;
        boost::asio::io_context&    _context;
        api::net::tcp::socket       _s;
        std::atomic_int32_t         _id;
        enum { max_length = 1024 };
        char                         _rbuffer[max_length];
    };
}