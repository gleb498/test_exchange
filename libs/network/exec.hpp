#pragma once

#include "messages.hpp"

#include <unordered_map>
#include <functional>

namespace exchange::net {
    class exec
    {
        using handler_t = std::function<std::shared_ptr<reply>(std::shared_ptr<msg_base>)>;

    public:
        exec() = default;
        void add_handler(api::msg_type, handler_t);
        std::shared_ptr<reply> execute(const std::string&) const;

    private:
        template<class MSG_T>
        void parse(const std::string&, std::shared_ptr<msg_base>) const;
        
    private:
        std::unordered_map<api::msg_type, handler_t> _handlers;
    };
}