#include "connection.hpp"
#include "exec.hpp"
#include "hub.hpp"

#include <api_app.hpp>
#include <spdlog/spdlog.h>

#include <boost/asio.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace exchange::net {
    connection::connection(api::app& app, api::net::socket_t s)
    :   _app(app),
        _exec(app.get_hub().commands()),
        _s(std::move(s)),
        _endpoint(_s.remote_endpoint())
    {}

    connection::~connection()
    {
        spdlog::debug("connection destroyed: {}", _endpoint.address().to_string() + ":" + std::to_string(_endpoint.port()));
    }

    void connection::start()
    {
        async_read();
    }

    void connection::stop()
    {
        if (!_s.is_open())
            return;

        _s.close();
    }

    void connection::async_read()
    {
        auto self = shared_from_this();
        _s.async_read_some( boost::asio::buffer(_rbuffer, max_length),
                            [this, self] (const boost::system::error_code& ec, size_t bytes_transferred)
                            {
                                handle_read(ec, bytes_transferred);
                            });
    }

    void connection::handle_read(   const boost::system::error_code& ec,
                                    size_t bytes_transferred)
    {
        if (ec)
            handle_error(ec);

        auto reply = _exec.execute(std::string(_rbuffer));

        if (!reply)
        {
            spdlog::error("there is no reply");
            async_read();
            return;
        }

        json res;
        to_json(res, *reply);

        std::string reply_str = res.dump();

        auto self = shared_from_this();
        boost::asio::async_write(_s,
                    boost::asio::buffer(reply_str, reply_str.size()),
                    [this, self] (const boost::system::error_code& ec, size_t)
                    {
                        handle_write(ec);
                    });
    }

    void connection::handle_write(const boost::system::error_code& ec)
    {
        if (ec)
            handle_error(ec);

        async_read();
    }

    void connection::handle_error(const boost::system::error_code& ec)
    {
        if (ec.value() != boost::asio::error::eof && ec.value() != boost::asio::error::bad_descriptor)
            spdlog::error("connection error: {}:{}", ec.value(), ec.message());
        
        _s.shutdown(api::net::socket_t::shutdown_both);
        _s.close();

        auto self = shared_from_this();
        on_disconect_event(self);
    }
}