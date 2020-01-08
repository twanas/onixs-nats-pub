#include "connection.h"
#include "publisher.h"
#include <chrono>
#include <thread>

void onMsg(natsConnection *nc, natsSubscription *sub, natsMsg *msg, void *closure)
{
    // Prints the message, using the message getters:
    printf("Received msg: %s - %.*s\n",
           natsMsg_GetSubject(msg),
           natsMsg_GetDataLength(msg),
           natsMsg_GetData(msg));

    // Don't forget to destroy the message!
    natsMsg_Destroy(msg);
}

int main()
{
    natsStatus s;
    natsConnection      *nc  = NULL;
    natsSubscription    *sub = NULL;
    natsMsg             *msg = NULL;

    const char* subject = "cme.md";

    s = natsConnection_ConnectTo(&nc, NATS_DEFAULT_URL);

    if (! s == NATS_OK)
    {
        std::cout << "Error connecting " << std::endl;
        exit(1);
    }

    natsConnection_Subscribe(&sub, nc, subject, onMsg, NULL);

    std::shared_ptr<Connection> conn = std::make_shared<Connection>();
    std::shared_ptr<Publisher> pub = std::make_shared<Publisher>(subject, conn);

    //for(auto i = 0; i<10; ++i)
    //    conn->publish("cme.md", "{\"inst\": \"fut-ES_201912\", \"bid\": 1300.25,  \"ask\": 1300.50}");

    while(1)
    {
        std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(2));
    }

    natsConnection_Destroy(nc);


}


/*
struct BBA
{
    int bidp;
    int askp;
    int bidv;
    int askv;
    int status;
};

struct Trade
{
    int price;
    int qty;
};

struct Publisher: public MarketDataListener
{
    Publisher(const Connection& c) {
        // handler.start();
    }

    ~Publisher()
    {
        // handler.stop();
    }

    void onConsolidatedBook(const ConsolidatedBook& book)
    {
        BBA = bba::build(book, status_[sec.id()]);
        conn->push_(bba.json());
    }

    void onTrade(const Trade& trade)
    {
        BBA = trade::build(trade);
        push_(bba.json());
    }

    void onStatus(const Status& status)
    {
        status_[sec.id()] = status.num;
    }

private:
    std::unordered_map<uint32_t, int> status_;
};

struct Connection
{
    Connection(const std::string& host): host_(host)
    {}
    void publish();
private:
    std::string host_;
};

struct Subscription
{
    std::string topic;
    uint16_t channel;
    std::vector<std::string> symbols;
};

struct Config
{
    std::string host;
    std::vector<std::string> subscriptions;
};

struct Service
{
    Service(const Config& cfg)
    {
        auto conn = std::make_shared<Connection>(cfg.host);

        for(const auto& sub: cfg.subscriptions)
        {
            subs_.emplace_back(
                    Publisher(sub, conn_);
            )
        }
    }

private:
    std::vector<Subscription> subs_;
}

int main()
{
    auto cfg = Config{
        "nats://xxxx.xxx.xxx",
        {
            "md.cme.310",
            310,
            {"ESU9", "ESZ9"}
        }
    };

    Service svc(cfg);
    // wait for keyboard interrupt
}
 */
