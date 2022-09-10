#pragma once

#include <api_msg.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace exchange::net {

    struct msg_base
    {
        msg_base() = default;
        msg_base(api::msg_type t) : type (t) {}
        api::msg_type get_type() { return type; }
        
    protected:
        api::msg_type type = api::msg_type::unknown;
    };

    struct msg_registration : public msg_base
    {
        msg_registration() : msg_base(api::msg_type::registration)
        {}

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(msg_registration, type)
    };

    struct msg_order : public msg_base
    {
        msg_order() : msg_base(api::msg_type::order)
        {}
        bool operator==(const msg_order& l) const;

        api::balance_t amount = 0;
        api::currency currency = api::currency::unknown;
        api::order_side side = api::order_side::unknown;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(msg_order, type, amount, currency, side)
    };

    struct msg_cancel_order : public msg_base
    {
        msg_cancel_order() : msg_base(api::msg_type::cancel_order)
        {}

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(msg_cancel_order, type)
    };

    //----------------------------------------------------------------------

    struct reply
    {
        reply() = default;
        reply(api::msg_type t) : type (t) {}
        api::msg_type type = api::msg_type::unknown;
        
        std::string msg;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(reply, type, msg)
    };
}