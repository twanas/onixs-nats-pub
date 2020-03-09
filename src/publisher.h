#pragma once

#include "connection.h"
#include "OnixS/CME/MDH.h"
#include <memory>
#include <array>
#include <sstream>

using namespace OnixS::CME::MDH;

void initialize() {
    InitializationSettings settings;
    settings.licenseStore(LICENSE_FOLDER);
    OnixS::CME::MDH::initialize(settings);
}

class Publisher : SecurityListener
{
public:
    explicit Publisher(const std::string &subject, std::shared_ptr<Connection> &conn)
            : subject_(subject)
              , conn_(conn) {

        FileLoggerSettings filelogger_cfg;
        filelogger_cfg.severityLevel(LogSeverity::Enum::Important);
        filelogger_cfg.filename("FeedHandler.log");
        file_logger_ = std::unique_ptr<FileLogger>(new FileLogger(filelogger_cfg));

        FeedEngineSettings engine_cfg;
        engine_cfg.socketBufferSize(8 * 1024 * 1024);
        engine_cfg.threadCount(8);
        engine_ = std::unique_ptr<FeedEngine>(new FeedEngine(engine_cfg));

        std::vector<int> channels{
                310,
                312,
                314,
                318,
                320,
                340,
                342,
                344,
                346,
                360,
                382,
                384,
                386
        };

        for (auto channel: channels) {
            HandlerSettings handler_cfg;
            handler_cfg.bookManagement().directBooks().maintain(true);
            handler_cfg.bookManagement().consolidatedBooks().maintain(false);
            handler_cfg.bookManagement().mboBooks().maintain(false);
            handler_cfg.feeds().connectivityConfigurationFile(CONNECTIVITY_CONFIGURATION_FILE);
            handler_cfg.channel(channel);
            handler_cfg.feeds().feedANetworkInterfaces(NETWORK_INTERFACE_A);
            handler_cfg.feeds().feedBNetworkInterfaces(NETWORK_INTERFACE_B);
          
            auto handler = std::unique_ptr<Handler>(new Handler(handler_cfg));
            handler->bindFeedEngine(*engine_);
            handler->bindLogger(*file_logger_);
            handler->registerSecurityListener(*this);

	    handlers_.push_back(std::move(handler));
        }

       for (auto& handler: handlers_) {
            SessionSettings session_cfg;
            handler->start(session_cfg);
       }
    }

    void onLimitsAndBanding(Handler &, const Security &sec, const LimitsAndBanding50Args &args) override {
        auto &msg = args.message();
        for (auto &entry: msg.entries()) {
            handleLimitsAndBanding(sec, entry);
        }
    }

    void onLimitsAndBanding(Handler &, const Security &sec, const LimitsAndBanding34Args &args) override {
        auto &msg = args.message();
        for (auto &entry: msg.entries()) {
            handleLimitsAndBanding(sec, entry);
        }
    }

    void onDefinition(Handler &, const Security &sec, const InstrumentDefinitionFuture54Args &args) override {
        handleLimitsAndBanding(sec, args.message());
    }

private:
    template<typename Entry>
    void handleLimitsAndBanding(const Security &sec, const Entry &entry) {

        Decimal high, low;
        if (entry.highLimitPrice(high) && entry.lowLimitPrice(low)) {

	    std::stringstream ss;
	    ss << "{\"symbol\": \"" << sec.symbol()
    	       << "\", \"high_limit\": " << toStr(high)
	       << "\", \"low_limit\": " << toStr(low)
	       << "}";

	    conn_->publish("cme.md", ss.str());
	}

    }

private:
    std::string subject_;
    std::vector<std::unique_ptr<Handler>> handlers_;
    std::unique_ptr<FeedEngine> engine_;
    std::unique_ptr<FileLogger> file_logger_;
    std::shared_ptr<Connection> conn_;
};
