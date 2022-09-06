#pragma once

#include <api_network.hpp>

#include <string>
#include <filesystem>
#include <boost/asio/ip/address.hpp>

namespace boost::asio::ip {
    template<class IStream>
    IStream& operator>>(IStream& in, address& addr)
    {
        std::string se;
        in >> se;
        addr = address_v4::from_string(se);
        return in;
    }
}

namespace exchange::config {

    class settings
    {
    public:
        settings();
        ~settings() = default;

        // return false if there is help params 
        bool parse(int argc, const char** argv);
        const boost::asio::ip::address& address() const;
        uint16_t port() const;
        api::net::endpoint_t endpoint() const;
        const std::filesystem::path& datadir() const;

    private:
        bool is_help(int argc, const char** argv);

    private:
        std::filesystem::path       _datadir;
        boost::asio::ip::address    _address;
        uint16_t                    _port;
    };
}