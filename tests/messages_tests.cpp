#include <catch.hpp>
#include <network/messages.hpp>

using namespace exchange;

TEST_CASE("msg::order", "[msg]")
{
    net::msg_order msg;
    msg.amount = 5;
    msg.currency = api::currency::RUB;
    msg.side = api::order_side::bid;

    json j;
    to_json(j, msg);
    
    CHECK(j["amount"] == 5);
    CHECK(j["currency"] == api::currency::RUB);
    CHECK(j["side"] == api::order_side::bid);
    CHECK(j["type"] == api::msg_type::order);

    std::string str = j.dump();

    net::msg_order msg2;
    from_json(json::parse(str), msg2);

    CHECK(msg == msg2);
}