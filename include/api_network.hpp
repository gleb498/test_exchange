#pragma once

#include <boost/asio/ip/tcp.hpp>

namespace exchange::api::net {

    using tcp = boost::asio::ip::tcp;

    using acceptor_t    = tcp::acceptor;
    using socket_t      = tcp::socket;
    using endpoint_t    = tcp::endpoint;
}
