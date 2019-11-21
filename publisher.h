#pragma once

#include "connection.h"
#include "OnixS/CME/MDH.h"
#include <memory>

using namespace OnixS::CME::MDH;

class Publisher: SecurityListener
{
public:
    explicit Publisher(const std::string& subject, std::shared_ptr<Connection>& conn)
    : subject_(subject)
    , conn_(conn)
    {
        SessionSettings session_cfg;
        HandlerSettings handler_cfg;
        handler_cfg.bookManagement().consolidatedBooks().bboTracking().enabled();
        handler_cfg.bookManagement().consolidatedBooks().maintain(true);
        handler_ = std::unique_ptr<Handler>(new Handler(handler_cfg));
        handler_->registerSecurityListener(*this);
        handler_->start(session_cfg);
    }

    virtual void onBookUpdate(Handler&, const Security&, const ConsolidatedBook&) override
    {
        conn_->publish(subject_, "{}");
    }

private:
    std::string subject_;
    std::unique_ptr<Handler> handler_;
    std::shared_ptr<Connection> conn_;
};
