#pragma once

#include "nats/nats.h"
#include <iostream>

class Connection
{
public:
    Connection()
    {
        status_ = natsConnection_ConnectTo(&conn_, NATS_DEFAULT_URL);

        if (status_ == NATS_OK)
            std::cout << "Connected to default host" << std::endl;
        else
            std::cout << "Error connecting to default host" << std::endl;
    }

    explicit Connection(const std::string& host)
    {
        status_ = natsConnection_ConnectTo(&conn_, host.c_str());

        if (status_ == NATS_OK)
            std::cout << "Connected to : " << host << std::endl;
        else
            std::cout << "Error connecting to : " << host << std::endl;
    }

    ~Connection()
    {
        natsConnection_Flush(conn_);
        natsConnection_Destroy(conn_);
    }

    void publish(const std::string& subject, char* data, size_t size)
    {
        if (status_ == NATS_OK)
            natsConnection_Publish(conn_, subject.c_str(), (const void*)data, size);
    }

    void publish(const std::string& subject, const std::string& text)
    {
        if (status_ == NATS_OK)
            natsConnection_PublishString(conn_, subject.c_str(), text.c_str());
    }

private:
   natsConnection* conn_ = nullptr;
   natsStatus status_;

};
