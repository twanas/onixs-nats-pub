#pragma once

#include "connection.h"
#include "OnixS/CME/MDH.h"
#include <memory>
#include <iostream>

using namespace OnixS::CME::MDH;

void initialize()
{
    InitializationSettings settings;
    settings.licenseStore(LICENSE_FOLDER);
    OnixS::CME::MDH::initialize(settings);
}

class Publisher: SecurityListener
{
public:
    explicit Publisher(const std::string& subject, std::shared_ptr<Connection>& conn)
    : subject_(subject)
    , conn_(conn)
    {

        FileLoggerSettings filelogger_cfg;
        filelogger_cfg.severityLevel(LogSeverity::Enum::Debug);
        filelogger_cfg.filename("Handler.log");
        file_logger_ = std::unique_ptr<FileLogger>(new FileLogger(filelogger_cfg));

        FeedEngineSettings engine_cfg;
        engine_cfg.socketBufferSize(8 * 1024 * 1024);
        engine_ = std::unique_ptr<FeedEngine>(new FeedEngine(engine_cfg));

        HandlerSettings handler_cfg;
        handler_cfg.bookManagement().consolidatedBooks().bboTracking().price().enabled(true);
        handler_cfg.bookManagement().consolidatedBooks().maintain(true);
        handler_cfg.bookManagement().mboBooks().maintain(false);
        handler_cfg.feeds().connectivityConfigurationFile(CONNECTIVITY_CONFIGURATION_FILE);
        handler_cfg.channel(310);
	
        handler_ = std::unique_ptr<Handler>(new Handler(handler_cfg));
        handler_->bindFeedEngine(*engine_);
	handler_->bindLogger(*file_logger_);
        handler_->registerSecurityListener(*this);

        SessionSettings session_cfg;
        handler_->start(session_cfg);
    }

    virtual void onBookUpdate(Handler&, const Security&, const ConsolidatedBook&) override
    {
	std::cout << "depth received" << std::endl;
        conn_->publish(subject_, "{ES}");
    }

private:
    std::string subject_;
    std::unique_ptr<Handler> handler_;
    std::unique_ptr<FeedEngine> engine_;
    std::unique_ptr<FileLogger> file_logger_;
    std::shared_ptr<Connection> conn_;
};