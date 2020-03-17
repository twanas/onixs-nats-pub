#pragma once

#include "connection.h"
#include "OnixS/CME/MDH.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <mutex>

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

//        std::unordered_map<ChannelId, SecurityGroupList> channels {
//                {310, {"ES"}},
//                {312, {"GE"}},
//                {314, {"6A", "6B", "6J", "6S"}},
//                {318, {"N1", "NQ", "RY"}},
//                {320, {"6C", "6E"}},
//                {340, {"ZC", "ZW"}},
//                {342, {"YM"}},
//                {344, {"ZB", "ZF", "ZN", "ZT", "ZU"}},
//                {346, {"ZS"}},
//                {360, {"GC", "HG", "SI"}},
//                {382, {"CL"}}
//        };

        std::unordered_map<ChannelId, SecurityGroupList> channels{
                {310, {"ESH1",  "ESH0",  "ESU0",  "ESZ0",  "ESM0"}},
                {342, {"YMZ0",  "YMM0",  "YMU0",  "YMH0"}},
                {318, {"NIYM2", "NIYU1", "NIYN0", "NIYJ0", "NIYK0", "NIYG0", "NIYM0", "NIYU0", "NIYU2", "NIYM1", "NIYZ1", "NIYZ0", "NIYZ5", "NIYH2", "NIYH0", "NIYH1", "RTYH1", "NQH1", "NQH0", "RTYH0", "RTYU0", "NQU0", "RTYZ0", "NQZ0", "NQM0", "RTYM0", "NIYZ3", "NIYZ4", "NIYZ2"}}
//                {340, { "ZCK2", "ZWK0", "ZWN1", "ZWH1", "ZCU2", "ZCU1", "ZCN1", "ZWZ1", "ZCH0", "ZCN3", "ZWH0", "ZCK1", "ZCH2", "ZWU0", "ZWZ0", "ZCZ0", "ZCZ1", "ZWU1", "ZWN0", "ZCH1", "ZWK2", "ZCK0", "ZCN0", "ZCZ2", "ZWK1", "ZCZ3", "ZWH2", "ZWN2", "ZCU0", "ZCN2" }},
//                {320, { "6CZ4", "6CN0", "6CZ0", "6CH4", "6CG0", "6CM1", "6CZ1", "6CU4", "6CK0", "6CU3", "6CZ2", "6EH1", "6CZ3", "6CU0", "6EG0", "6CJ0", "6EK0", "6CH1", "6CU2", "6EJ0", "6CH3", "6EM2", "6EM3", "6EN0", "6CU1", "6CH2", "6CH0", "6EH2", "6CM4", "6CM0", "6EU2", "6EH0", "6EM0", "6EM1", "6EZ1", "6EZ0", "6EU0", "6EM4", "6EZ4", "6CM3", "6EH4", "6EU3", "6EZ2", "6CM2", "6EU4", "6EU1", "6EH3", "6EZ3" }},
//                {382, { "CLK0", "CLQ4", "CLH5", "CLF5", "CLQ30", "CLM3", "CLU5", "HOU0", "RBQ3", "RBJ3", "RBZ3", "CLF1", "CLZ30", "CLV3", "CLF4", "HOH3", "CLF3", "CLN0", "CLK4", "HOK1", "HON1", "HOK3", "CLN7", "CLK2", "CLM5", "CLG2", "RBX0", "RBX1", "HOF2", "RBM1", "CLX7", "HOM2", "CLK6", "CLH1", "CLH8", "CLV8", "CLX8", "CLN9", "CLU9", "CLG30", "RBJ0", "CLX1", "CLH7", "HOG2", "CLH6", "CLN4", "RBN2", "RBK2", "RBV2", "RBZ2", "HOQ0", "CLZ0", "CLQ5", "CLQ9", "RBQ0", "CLZ3", "HOU2", "HOZ0", "CLN30", "CLX4", "CLJ8", "CLX0", "CLH2", "CLF6", "RBX3", "CLF30", "CLX2", "HOF3", "RBV1", "HOF1", "CLM1", "CLG3", "CLZ4", "CLX30", "CLN8", "CLU3", "CLQ2", "RBK1", "CLJ4", "RBM3", "HOV3", "CLK3", "CLJ9", "CLV0", "CLQ1", "RBU1", "CLX6", "RBF2", "HOJ1", "HOZ1", "HOJ3", "CLM4", "CLM9", "RBZ0", "RBX2", "CLM2", "CLG1", "CLQ6", "HON0", "CLG31", "HOQ3", "CLV7", "CLG6", "HOX0", "CLV4", "CLN5", "RBV3", "CLV9", "RBN0", "CLZ7", "HOK2", "HOJ0", "CLZ1", "RBJ2", "CLK5", "CLN2", "CLX5", "RBN3", "CLZ9", "CLV2", "CLF8", "HOZ2", "RBH0", "RBJ1", "CLZ6", "CLM30", "CLK8", "CLF2", "CLH4", "RBZ1", "HOH1", "RBH3", "HOQ2", "HOU3", "CLJ3", "CLV30", "CLQ8", "CLQ3", "CLU0", "CLN1", "RBQ1", "CLF7", "CLZ2", "CLJ0", "HOV1", "HOX1", "RBU2", "CLK9", "HOM1", "RBM2", "HOF4", "CLH9", "CLK1", "CLU6", "RBV0", "RBG1", "CLH0", "HOM0", "CLK7", "CLJ30", "RBM0", "HOM3", "CLU7", "HOJ2", "HOV0", "CLV1", "CLU4", "RBU3", "CLG9", "HOX2", "HOH0", "RBH2", "CLG5", "CLJ2", "CLJ5", "RBK3", "CLX9", "CLG8", "HON2", "RBH1", "CLK30", "CLM8", "CLZ5", "CLM0", "RBN1", "CLV5", "HOG1", "RBG3", "CLU2", "RBF4", "HOX3", "CLM6", "CLU30", "CLU8", "CLN3", "CLG4", "HOU1", "RBQ2", "HOZ3", "CLH3", "CLF31", "CLF9", "CLX3", "CLV6", "RBF3", "HOQ1", "HON3", "CLQ7", "CLZ8", "CLJ1", "HOG3", "RBU0", "CLH30", "RBK0", "CLM7", "HOH2", "CLJ6", "RBG2", "CLU1", "CLN6", "HOK0", "RBF1", "CLJ7", "HOV2", "CLG7", "CLQ0"}},
//                {360, { "HGF1", "GCQ1", "HGG1", "GCM5", "HGV1", "GCZ4", "GCZ0", "GCM4", "SIN2", "GCM0", "GCM2", "SIZ3", "HGK4", "SIH0", "GCV1", "SIJ0", "SIG0", "HGX1", "HGF2", "GCJ1", "HGQ1", "HGK1", "HGH2", "GCQ0", "HGH3", "HGN1", "GCG1", "GCZ3", "SIK0", "HGK3", "HGM1", "HGV0", "HGG0", "GCH0", "SIF1", "GCV0", "HGN2", "HGX0", "HGJ0", "SIN0", "SIZ1", "HGU4", "SIU1", "GCM3", "HGH4", "SIN1", "HGQ0", "HGZ3", "HGK2", "GCZ5", "SIZ4", "SIN3", "GCJ0", "HGM0", "GCZ1", "SIU0", "HGJ1", "SIH1", "SIK1", "HGU2", "HGN4", "SIN4", "GCM1", "GCG0", "SIZ2", "HGN3", "HGU3", "HGH0", "HGZ4", "HGK0", "SIZ0", "HGU0", "HGZ2", "HGZ0", "GCZ2", "HGZ1", "HGU1", "HGH1", "HGN0"}},
//                {344, { "UBM0", "ZNM0", "ZBM0", "UBU0", "ZBU0", "ZTM0", "ZFM0", "ZFH0", "ZNU0", "ZTH0", "ZFU0", "UBH0", "ZBH0", "ZTU0", "ZNH0" }},
//                {346, { "ZLV0", "ZMZ3", "ZLU2", "ZSN3", "ZMQ2", "ZMK1", "ZLV1", "ZLF2", "ZSH2", "ZSH1", "ZSU0", "ZMN2", "ZLH0", "ZLH1", "ZLZ2", "ZMN1", "ZSH0", "ZMV1", "ZMQ0", "ZSX0", "ZSN1", "ZMF2", "ZMU2", "ZMH1", "ZLQ2", "ZSU2", "ZSF1", "ZSN0", "ZLZ3", "ZSU1", "ZLV2", "ZLK0", "ZLK1", "ZSX1", "ZLZ1", "ZMV0", "ZMU0", "ZSQ0", "ZMH0", "ZMZ0", "ZLN0", "ZLZ0", "ZMV3", "ZSX3", "ZMK2", "ZMF1", "ZLV3", "ZSQ2", "ZSX2", "ZMU1", "ZLQ0", "ZLK2", "ZLF1", "ZSQ1", "ZLN2", "ZLN1", "ZLU1", "ZMK0", "ZSK0", "ZMN3", "ZLN3", "ZMH2", "ZMZ2", "ZLU0", "ZLH2", "ZSF2", "ZSN2", "ZMQ1", "ZLQ1", "ZSK2", "ZSK1", "ZMV2", "ZMN0", "ZMZ1" }},
//                {314, { "6SH1", "6AH1", "6JH1", "6BG0", "6JG0", "6JK0", "6BK0", "6AG0", "6AK0", "6AJ0", "6BJ0", "6JJ0", "6BH1", "6SM2", "6BM2", "6JM2", "6AM2", "6AM3", "6BM3", "6SM3", "6JM3", "6BN0", "6AN0", "6JN0", "6SH0", "6BH0", "6JH2", "6BH2", "6AH2", "6SH2", "6JH0", "6JU2", "6AU2", "6SU2", "6BU2", "6AH0", "6JM1", "6JM0", "6BM0", "6AM0", "6SM1", "6AM1", "6BM1", "6SZ1", "6BZ1", "6JZ1", "6AZ1", "6SM0", "6BZ0", "6SZ0", "6AU0", "6AZ0", "6BU0", "6JZ0", "6SU0", "6JU0", "6AM4", "6JM4", "6BM4", "6SM4", "6JZ4", "6BZ4", "6SZ4", "6AZ4", "6BH4", "6SH4", "6JH4", "6AH4", "6SU3", "6JU3", "6BU3", "6AU3", "6BZ2", "6AZ2", "6SZ2", "6JZ2", "6BU4", "6SU4", "6AU4", "6JU4", "6AU1", "6BU1", "6SU1", "6JU1", "6JH3", "6AH3", "6SH3", "6BH3", "6SZ3", "6JZ3", "6AZ3", "6BZ3" }},
//                {312, { "GEH4", "GEN0", "GEH6", "GEM4", "GEZ9", "GEM2", "GEU2", "GEH3", "GEJ0", "GEQ0", "GEH7", "GEU3", "GEU8", "GEM3", "GEU4", "GEZ6", "GEU7", "GEU9", "GEZ7", "GEK0", "GEZ1", "GEG0", "GEH8", "GEH2", "GEU1", "GEH5", "GEM9", "GEH9", "GEM7", "GEU6", "GEM8", "GEM5", "GEZ8", "GEM6", "GEU5", "GEZ5", "GEZ4", "GEZ3", "GEZ2", "GEH0", "GEU0", "GEM0", "GEM1", "GEZ0", "GEH1" }},
//                {384, { "PAM0", "PAG0", "PAH1", "PLH0", "PLG0", "PAZ0", "PLN0", "PLJ1", "PAJ0", "PAH0", "PAU0", "PLV0", "PLF1", "PLJ0" }},
//                {386, {"NGZ27", "NGX27", "NGV27", "NGU27", "NGQ27", "NGN27", "NGM27", "NGK27", "NGJ27", "NGG27", "NGH27", "NGZ25", "NGX25", "NGV25", "NGU25", "NGQ25", "NGN25", "NGM5", "NGK5", "NGJ5", "NGH5", "NGG5", "NGF5", "NGZ4", "NGX4", "NGV4", "NGU4", "NGQ4", "NGN4", "NGM4", "NGK4", "NGJ4", "NGH4", "NGZ26", "NGX26", "NGV26", "NGU26", "NGQ26", "NGN26", "NGM26", "NGK26", "NGG26", "NGJ26", "NGF26", "NGH26", "NGF27", "NGF32", "NGG32", "NGV28", "NGX28", "NGH32", "NGJ32", "NGV32", "NGX32", "NGZ32", "NGZ28", "NGF28", "NGG28", "NGH28", "NGJ28", "NGK28", "NGN28", "NGK32", "NGM28", "NGM32", "NGN32", "NGQ32", "NGU32", "NGQ28", "NGU28", "NGF31", "NGG31", "NGH31", "NGJ31", "NGK31", "NGM31", "NGN31", "NGU30", "NGQ31", "NGV31", "NGU31", "NGX31", "NGZ31", "NGF30", "NGG30", "NGH30", "NGJ30", "NGK30", "NGM30", "NGN30", "NGQ30", "NGF4", "NGG4", "NGV30", "NGX30", "NGZ30", "NGV3", "NGH0", "NGJ0", "NGK0", "NGM0", "NGN0", "NGQ0", "NGU0", "NGV0", "NGX0", "NGZ0", "NGF1", "NGG1", "NGH1", "NGJ1", "NGK1", "NGM1", "NGN1", "NGQ1", "NGU1", "NGV1", "NGX1", "NGZ1", "NGZ3", "NGU3", "NGG2", "NGZ2", "NGK3", "NGQ3", "NGJ3", "NGF2", "NGX3", "NGF3", "NGH2", "NGJ2", "NGN3", "NGN2", "NGM3", "NGX2", "NGG3", "NGM2", "NGQ2", "NGK2", "NGU2", "NGV2", "NGH3", "NGG29", "NGF29", "NGH29", "NGJ29", "NGK29", "NGM29", "NGN29", "NGQ29", "NGU29", "NGV29", "NGX29", "NGZ29"}},
        };

        FileLoggerSettings filelogger_cfg;
        filelogger_cfg.severityLevel(LogSeverity::Enum::Important);
        filelogger_cfg.filename("FeedHandler.log");
        file_logger_ = std::unique_ptr<FileLogger>(new FileLogger(filelogger_cfg));

        FeedEngineSettings engine_cfg;
        engine_cfg.socketBufferSize(64 * 1024 * 1024);
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
            for (auto &symbol: channel.second) {
                filter.symbols().insert(toStrRef(symbol));
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
        handleLimitsAndBanding(sec, args.entry(), args.message().transactTime());
    }

    void onLimitsAndBanding(Handler &, const Security &sec, const LimitsAndBanding34Args &args) override {
        handleLimitsAndBanding(sec, args.entry(), args.message().transactTime());
    }

    void onRecovery(Handler &, const Security &sec, const SnapshotFullRefresh38Args &args) override {
        handleLimitsAndBanding(sec, args.message(), args.message().transactTime());
    }

    void onRecovery(Handler &, const Security &sec, const SnapshotFullRefresh52Args &args) override {
        handleLimitsAndBanding(sec, args.message(), args.message().transactTime());
    }

    void onPacket(Handler &, const PacketArgs &packet) override {
        sendingtime_ = packet.sendingTime();
    }


private:
    template<typename Entry>
    void handleLimitsAndBanding(const Security &sec, const Entry &entry, const Timestamp &transacttime) {

        std::lock_guard<std::mutex> lock(mtx_);

        bool high_or_low{false};

        std::stringstream ss;
        ss << "{\"sendingtime\": \"" << toStr(sendingtime_) << "\"";
        ss << ",\"transacttime\": \"" << toStr(transacttime) << "\"";
        ss << ",\"symbol\": \"" << sec.symbol() << "\"";

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
    std::mutex mtx_;
    OnixS::CME::MDH::Timestamp sendingtime_;
};
