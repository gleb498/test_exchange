#include "exec.hpp"

#include <spdlog/spdlog.h>

namespace exchange::net {
    void exec::add_handler(api::msg_type t, handler_t h)
    {
        if (_handlers.count(t))
            throw std::runtime_error("try to add same handler twice");

        _handlers.emplace(t, h);
    }

    std::shared_ptr<reply> exec::execute(const std::string& raw) const
    {
        auto j = json::parse(raw);

        if (!j.contains("type"))
        {
            spdlog::error("no type in received msg");
            return {};
        }

        api::msg_type type = static_cast<api::msg_type>(j["type"]);

        std::shared_ptr<msg_base> msg = nullptr;

        switch (type) 
        {
        case api::msg_type::registration:
            parse<msg_registration>(raw, msg);
            break;
        case api::msg_type::order:
            parse<msg_order>(raw, msg);
            break;
        case api::msg_type::cancel_order:
            parse<msg_cancel_order>(raw, msg);
            break;
        default:
            spdlog::error("unknown msg type");
            return nullptr;
        }

        return _handlers.at(type)(msg);
    }

    template<class MSG_T>
    void exec::parse(const std::string& raw, std::shared_ptr<msg_base> msg) const
    {
        MSG_T order;
        from_json(json::parse(raw), order);
        msg = std::make_shared<MSG_T>(order);
    }
}