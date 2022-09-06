#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/chrono.h>

namespace exchange::core {
    void initialize_log(const char* name, bool async = true);
    void shutdown_log();
}

