#include "messages.hpp"

namespace exchange::net {

    bool msg_order::operator==(const msg_order& l) const
    {
        return std::tie(type, amount, currency, side) == std::tie(l.type, l.amount, l.currency, l.side);
    }
}