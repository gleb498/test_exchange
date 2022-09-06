#include "log.hpp"

#include <cstdint>
#include <fstream>
#include <vector>

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

const constexpr uint32_t    flush_interval = 5;

namespace exchange::core {

    void initialize_log(const char* loger_name, bool async)
    {
        // queue with 8k items and 1 backing thread
        spdlog::init_thread_pool(8192, 1);

        spdlog::level::level_enum global = spdlog::get_level();
        spdlog::set_level(global);

        std::vector<spdlog::sink_ptr> sinks;

        // console logger
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_st>();
        console_sink->set_level(global);
        sinks.emplace_back(console_sink);

        if(async)
        {
            auto logger = std::make_shared<spdlog::async_logger>(loger_name, sinks.begin(), sinks.end(),
                            spdlog::thread_pool(), spdlog::async_overflow_policy::block);
            logger->set_level(global);
            spdlog::set_default_logger(logger);
        }
        else
        {
            auto logger = std::make_shared<spdlog::logger>(loger_name, sinks.begin(), sinks.end());
            logger->set_level(global);
            spdlog::set_default_logger(logger);
        }

        spdlog::flush_every(std::chrono::seconds(flush_interval));
        spdlog::flush_on(spdlog::level::info);
        spdlog::flush_on(spdlog::level::err);
    }

    void shutdown_log()
    {
        spdlog::default_logger()->flush();
        spdlog::shutdown();
        spdlog::drop_all();
    }
}