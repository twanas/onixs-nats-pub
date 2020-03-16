#pragma once

#include "connection.h"
#include "OnixS/CME/MDH.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <sstream>

using namespace OnixS::CME::MDH;

void initialize() {
    InitializationSettings settings;
    settings.licenseStore(LICENSE_FOLDER);
    OnixS::CME::MDH::initialize(settings);
}

using SecurityGroupList = std::vector<std::string>;

class Publisher : SecurityListener, MarketDataListener
{
public:
    explicit Publisher(const std::string &subject, std::shared_ptr<Connection> &conn)
            : subject_(subject)
              , conn_(conn) {

        std::unordered_map<ChannelId, SecurityGroupList> channels {
                {310, {"ES"}},
                {312, {"GE"}},
                {314, {"6A", "6B", "6J", "6S"}},
                {318, {"N1", "NQ", "RY"}},
                {320, {"6C", "6E"}},
                {340, {"ZC", "ZW"}},
                {342, {"YM"}},
                {344, {"ZB", "ZF", "ZN", "ZT", "ZU"}},
                {346, {"ZS"}},
                {360, {"GC", "HG", "SI"}},
                {382, {"CL"}}
        };

        FileLoggerSettings filelogger_cfg;
        filelogger_cfg.severityLevel(LogSeverity::Enum::Important);
        filelogger_cfg.filename("FeedHandler.log");
        file_logger_ = std::unique_ptr<FileLogger>(new FileLogger(filelogger_cfg));

        FeedEngineSettings engine_cfg;
        engine_cfg.socketBufferSize(8 * 1024 * 1024);
        engine_cfg.threadCount(channels.size());
        engine_ = std::unique_ptr<FeedEngine>(new FeedEngine(engine_cfg));

        for (auto channel: channels) {
            HandlerSettings handler_cfg;
            handler_cfg.bookManagement().directBooks().maintain(false);
            handler_cfg.bookManagement().consolidatedBooks().maintain(false);
            handler_cfg.bookManagement().mboBooks().maintain(false);
            handler_cfg.feeds().connectivityConfigurationFile(CONNECTIVITY_CONFIGURATION_FILE);
            handler_cfg.channel(channel.first);
            handler_cfg.feeds().feedANetworkInterfaces(NETWORK_INTERFACE_A);
            handler_cfg.feeds().feedBNetworkInterfaces(NETWORK_INTERFACE_B);

            auto handler = std::unique_ptr<Handler>(new Handler(handler_cfg));
            handler->bindFeedEngine(*engine_);
            handler->bindLogger(*file_logger_);
            handler->registerSecurityListener(*this);
            handler->registerMarketDataListener(*this);

            InstrumentSelection filter;
            for (auto &sec_group: channel.second) {
                filter.securityGroups().insert(toStrRef(sec_group));
            }
            handler->selectInstruments(filter);

            handlers_.push_back(std::move(handler));
        }

        for (auto &handler: handlers_) {
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

    void onDefinition(Handler &, const Security &sec, const InstrumentDefinitionFuture27Args &args) override {
        handleLimitsAndBanding(sec, args.message());
    }

    void onPacket(Handler &, const PacketArgs &packet) override {
        sendingtime_ = packet.sendingTime();
    }


private:
    template<typename Entry>
    void handleLimitsAndBanding(const Security &sec, const Entry &entry) {

        bool high_or_low{false};

        std::stringstream ss;
//        ss.clear();

        ss << "{\"sendingtime\": \"" << toStr(sendingtime_) << "\"";
        ss << ", \"symbol\": \"" << sec.symbol() << "\"";

        Decimal high, low;
        if (entry.highLimitPrice(high)) {
            ss << ", \"high_limit\": " << toStr(high);
            high_or_low = true;
        }
        if (entry.lowLimitPrice(low)) {
            ss << ", \"low_limit\": " << toStr(low);
            high_or_low = true;
        }

        ss << "}";

        if (high_or_low) {
            conn_->publish("cme.md", ss.str());
        }

    }

private:
    std::string subject_;
    std::vector<std::unique_ptr<Handler>> handlers_;
    std::unique_ptr<FeedEngine> engine_;
    std::unique_ptr<FileLogger> file_logger_;
    std::shared_ptr<Connection> conn_;
    OnixS::CME::MDH::Timestamp sendingtime_;
};
