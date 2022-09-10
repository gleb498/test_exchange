#include "console.hpp"
#include "application.hpp"

#include <network/client.hpp>
#include <network/messages.hpp>

#include <cli/filehistorystorage.h>

#include <spdlog/spdlog.h>

namespace exchange::cli {
    console::console(boost::asio::io_context& context, application& app)
    :   _app(app),
        _client(app.get_client()),
        _scheduler(context)
    {
        // _rootMenu->Insert(
        //     "order",
        //     [this](std::ostream& out,   api::balance_t amount, 
        //                                 const std::string& cur, 
        //                                 const std::string& side)
        //     {
        //         net::msg_order order;
        //         order.amount = amount;

        //         if (cur == "RUB")
        //             order.currency = api::currency::RUB;
        //         else if (cur == "USD")
        //             order.currency = api::currency::USD;
        //         else
        //         {
        //             _cli->cout() << "Unknown currency" << std::endl;
        //             _local_session->Prompt();
        //             return;
        //         }

        //         if (side == "ask")
        //             order.side = api::order_side::ask;
        //         else if (side == "bid")
        //             order.side = api::order_side::bid;
        //         else
        //         {
        //             _cli->cout() << "Unknown order side" << std::endl;
        //             _local_session->Prompt();
        //             return;
        //         }

        //         json msg;
        //         to_json(msg, order);

        //         _client.call(msg.dump());
        //     }
        // );

        // _rootMenu->Insert(
        //     "cancel_order",
        //     [this](std::ostream& out, std::string type){
        //         _client.call("");
        //     }
        // );
    }

    void console::start()
    {
        _client.on_connect_event.connect([this]()
        { 
            run(); 
        });

        _client.on_msg_event.connect([](){});
    }

    void console::stop()
    {
        _client.on_connect_event.disconnect_all_slots();
        _client.on_msg_event.disconnect_all_slots();

        _scheduler.Stop();
    }

    void console::run()
    {
        _cli = std::make_unique<::cli::Cli>(std::move(_rootMenu), std::make_unique<::cli::FileHistoryStorage>("./.exchange_cli_history"));

        _local_session = std::make_unique<::cli::CliLocalTerminalSession>(*_cli, _scheduler, std::cout, 200);

        _local_session->ExitAction([this](auto& out){
            stop();
        });
    }

    void console::on_reply() const
    {
        _cli->cout() << "" << std::endl;
        _local_session->Prompt();
    }
}