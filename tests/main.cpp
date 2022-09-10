#define CATCH_CONFIG_RUNNER 1
#include <catch.hpp>

#include <core/log.hpp>

#include <string>
#include <vector>
#include <iostream>

int main( int argc, char* argv[])
{
    int result = 0;
    try
    {   
        exchange::core::initialize_log("test");
        result = Catch::Session().run(argc, argv);
    }
    catch(const std::exception& e)
    {
        spdlog::critical("unhandled exception: {}", e.what());
        result = 1;
    }

    exchange::core::shutdown_log();
    
    return result;
}