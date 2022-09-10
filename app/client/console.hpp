#pragma once

#include <core/factory.hpp>

#include <cli/boostasioscheduler.h>
#include <cli/clilocalsession.h>
#include <cli/cli.h>

#include <boost/asio/io_context.hpp>

namespace exchange {
    class application;
}

namespace exchange::net {
    class client;
}

namespace exchange::cli {
    class console : public core::factory<console>
    {
        friend class core::factory<console>;

    private:
        console(boost::asio::io_context& context, application& app);

    public:
        ~console() = default;
        
        void start();
        void stop();

        void on_reply() const;

    private:
        void run();

    private:
        application&                    _app;
        std::unique_ptr<::cli::Menu>    _rootMenu;
        net::client&                    _client;

        ::cli::BoostAsioScheduler                         _scheduler;
        std::unique_ptr<::cli::Cli>                       _cli;
        std::unique_ptr<::cli::CliLocalTerminalSession>   _local_session;
    };
}