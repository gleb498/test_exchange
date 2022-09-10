#pragma once

#include <cstdint>

namespace exchange::api {
    using balance_t = int32_t;
    
    enum class msg_type : uint8_t
    {
        unknown,
        registration,
        order,
        cancel_order
    };

    enum class order_side : uint8_t
    {
        unknown,
        bid,    // buy
        ask     // sell
    };

    enum class currency : uint8_t
    {
        unknown,
        RUB,
        USD
    };
}