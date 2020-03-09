#include "connection.h"
#include "publisher.h"
#include <chrono>
#include <thread>

void onMsg(natsConnection *nc, natsSubscription *sub, natsMsg *msg, void *closure)
{
    printf("Received msg: %s - %.*s\n",
           natsMsg_GetSubject(msg),
           natsMsg_GetDataLength(msg),
           natsMsg_GetData(msg));

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

    for(;;)
    {
        std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(2));
    }

    natsConnection_Destroy(nc);


}
