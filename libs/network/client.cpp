#include "client.hpp"
#include <exchange/application.hpp>
#include <config/settings.hpp>

#include <boost/asio/write.hpp>
#include <boost/asio/read_until.hpp>

#include <sstream>
#include <spdlog/spdlog.h>

namespace exchange::net {

    client::client(boost::asio::io_context& context, exchange::application& app)
    :   _app(app),
        _context(context),
        _s(_context)
    {}

    void client::connect()
    {
        api::net::tcp::resolver r(_context);
        _app.get_context();
        _app.get_settings();
        _app.get_settings().endpoint();
        api::net::tcp::resolver::query query(api::net::tcp::v4(),   _app.get_settings().address().to_string(),
                                                                    std::to_string(_app.get_settings().port()));
        api::net::tcp::resolver::iterator it = r.resolve(query);
        start_connect(it);
    }

    void client::start_connect(api::net::tcp::resolver::iterator endpoint_iter)
    {
        if (endpoint_iter != api::net::tcp::resolver::iterator())
        {
            _s.async_connect(endpoint_iter->endpoint(), 
                            [this, &endpoint_iter] (const boost::system::error_code& ec) 
                            { 
                                handle_connect(ec, endpoint_iter);
                            });
        }
    }

    void client::handle_connect(const boost::system::error_code& ec,
                                api::net::tcp::resolver::iterator endpoint_iter)
    {
        if (!_s.is_open())
            start_connect(++endpoint_iter);
        else if (ec)
        {
            // close old before starting a new one
            _s.close();
            start_connect(++endpoint_iter);
        }
        else
        {
            spdlog::info("connected to {}:{}",  endpoint_iter->endpoint().address().to_string(), 
                                                std::to_string(endpoint_iter->endpoint().port()));

            on_connect_event();

            call("ping");
        }
    }

    void client::start_read()
    {
        _s.async_read_some(boost::asio::buffer(_rbuffer, max_length),
                            [this](const boost::system::error_code& ec, size_t)
                            {
                                handle_read(ec);
                            });
    }

    void client::handle_read(const boost::system::error_code& ec)
    {
        if (!ec)
        {
            spdlog::info("received: {}", std::string(_rbuffer));

            on_msg_event();

            start_read();
        }
        else
            handle_error(ec);
    }

    void client::call(const std::string& msg)
    {
        _s.async_write_some(boost::asio::buffer(msg.c_str(), msg.size()),
                [this](const boost::system::error_code& ec, size_t)
                {
                    handle_write(ec);
                });
    }

    void client::handle_write(const boost::system::error_code& ec)
    {
        if (ec)
        {
            handle_error(ec);
            return;
        }

        start_read();
    }

    void client::handle_error(const boost::system::error_code& ec)
    {
        if (ec.value() == boost::asio::error::operation_aborted)
        {
            spdlog::info("client closed...");
        }

        _app.get_context().post([this](){ _app.stop(); });
    }

    void client::stop()
    {
        if (!_s.is_open())
            return;
        _s.close();
    }
}