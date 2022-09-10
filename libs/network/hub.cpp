#include "hub.hpp"

#include "server.hpp"

#include <api_app.hpp>
#include <api_msg.hpp>

namespace exchange::net {

    hub::hub(api::app& app)
    :   _app(app),
        _server(net::server::create(app)),
        _exec()
    {
        _exec.add_handler(api::msg_type::registration, std::bind(&hub::on_registration, this, std::placeholders::_1));
        _exec.add_handler(api::msg_type::order, std::bind(&hub::on_order, this, std::placeholders::_1));
        _exec.add_handler(api::msg_type::cancel_order, std::bind(&hub::on_cancel_order, this, std::placeholders::_1));
    }

    void hub::start()
    {
        _server->start();
    }

    void hub::stop()
    {
        _server->stop();
    }

    exec& hub::commands()
    {
        return _exec;
    }

    std::shared_ptr<reply> hub::on_registration(std::shared_ptr<msg_base>)
    {
        return {};
    }

    std::shared_ptr<reply> hub::on_order(std::shared_ptr<msg_base>)
    {
        std::shared_ptr<reply> r = std::make_shared<reply>();
        r->msg = "on oreder reply!";
        return r;
    }

    std::shared_ptr<reply> hub::on_cancel_order(std::shared_ptr<msg_base>)
    {
        return {};
    }
}