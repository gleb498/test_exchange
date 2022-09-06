#include "settings.hpp"
#include <boost/program_options.hpp>

#include <iostream>

namespace exchange::config {
    
    settings::settings()
    :   _datadir(std::filesystem::path("./exchange")),
        _address(boost::asio::ip::address_v4::any()),
        _port(10000)
    {

    }

    bool settings::parse(int argc, const char **argv)
    {
        using namespace boost::program_options;

        options_description desc("main");
        desc.add_options()
        ("help,h", "show config parameters")
        ("data-dir,d", value<std::filesystem::path>(&_datadir), "data directory")
        ("address,a", value<boost::asio::ip::address>(&_address), "server address")
        ("port,p", value<uint16_t>(&_port), "server port");

        variables_map vm;
        store(parse_command_line(argc, argv, desc), vm);
        notify(vm);

        if (is_help(argc, argv))
        {
            std::cout << "Parameters: \n" << desc << std::endl;
            return false;
        }

        return true;
    }

    const boost::asio::ip::address& settings::address() const
    {
        return _address;
    }

    uint16_t settings::port() const
    {
        return _port;
    }

    api::net::endpoint_t settings::endpoint() const
    {
        return api::net::endpoint_t(_address, _port);
    }
    
    const std::filesystem::path& settings::datadir() const
    {
        return _datadir;
    }

    bool settings::is_help(int argc, const char** argv)
    {
        using namespace boost::program_options;

        options_description desc("help");
        desc.add_options() ("help,h", "");
        variables_map vm;
        store(command_line_parser(argc, argv).options(desc).allow_unregistered().run(), vm);
        notify(vm);

        if (vm.count("help"))
            return true;

        return false;
    }
}