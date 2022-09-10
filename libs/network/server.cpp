#include "server.hpp"
#include "connection.hpp"

#include <api_app.hpp>
#include <config/settings.hpp>

#include <spdlog/spdlog.h>

namespace exchange::net {

    server::server(api::app& app)
    :   _app(app),
        _local(app.get_settings().endpoint()),
        _s(app.get_context())
    {}

    void server::start()
    {
        if(_s.is_open())
            return;

        _s.open(_local.protocol());

        if(_local.address().is_v4())
        {
            spdlog::info("starting server at {}:{}",
                _local.address().to_string(),
                _local.port());

            try
            {
                _s.bind(_local);
                _s.listen();
                start_accept();
            }
            catch(const boost::system::system_error& error)
            {
                handle_error(error.code());
            }
        }
        else
        {
            auto error = boost::system::errc::make_error_code(boost::system::errc::function_not_supported);
            handle_error(error);
        }
    }

    void server::stop()
    {
        std::unique_lock ul(_mutex);
        for (auto& c: _conns)
            c->stop();
        
        _conns.clear();
        ul.unlock();
        
        if (!_s.is_open())
            return;

        _s.close();
    }

    void server::start_accept()
    {
        auto self = shared_from_this();
        _s.async_accept([this, self](const boost::system::error_code& error, api::net::socket_t peer)
        {
            if (error)
                handle_error(error);
            else
                handle_accept(std::move(peer));
        });
    }

    void server::handle_accept(api::net::socket_t&& s)
    {
        auto end = s.remote_endpoint();
        spdlog::info("accepted from {}", end.address().to_string() + ":" + std::to_string(end.port()));
        auto c = net::connection::create(_app, std::move(s));
        
        std::unique_lock ul(_mutex);
        _conns.emplace(c);
        _mutex.unlock();

        c->on_disconect_event.connect([this](std::shared_ptr<connection> con)
        {
            spdlog::info("connection {}:{} closed", con->endpoint().address().to_string(), con->endpoint().port());
            std::unique_lock ul(_mutex);
            _conns.erase(con);
            _mutex.unlock();
        });

        _app.get_context().post([c]() { c->start(); });
        start_accept();
    }

    void server::handle_error(const boost::system::error_code& error)
    {   
        if (error.value() == boost::asio::error::operation_aborted)
            spdlog::info("server closed...");
        else
        {
            spdlog::critical("server error: {} - {}", error.value(), error.message());
            _app.get_context().post([this](){ _app.stop(); });
        }
    }
}