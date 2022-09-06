#include "application.hpp"
#include <core/log.hpp>
#include <config/settings.hpp>
#include <iostream>

int main(int argc, const char** argv)
{
    int res = 0;
    
    try
    {
        exchange::core::initialize_log("exchange");
        exchange::config::settings cfg;
        if (!cfg.parse(argc, argv))
            return res;

        exchange::application app(cfg);
        app.run();
    }
    catch (std::exception& e)
    {
        spdlog::error("exchange critial error: {}", e.what());
        res = 1;
    }

    exchange::core::shutdown_log();
    return res;
}