#include "libftpp.hpp"

int main(void)
{
    Server              server;
    std::thread         serverThread(&Server::start, &server, 7777);
    std::vector<Client> clients;
    Message             ping(1);
    ping << "ping";
    for (int j = 0; j < 5; j++)
    {
        for (int i = 0; i < 500; i++)
        {
            clients.emplace_back("127.0.0.1", 7777);
            // clients.back().connect("127.0.0.1", 7777);
            clients.back().defineAction(1,
                                        [](const Message& msg)
                                        {
                                            std::string value;
                                            value = msg.messageToString();
                                            std::cout << "Received: " << value << std::endl;
                                        });
        }
        for (int i = 0; i < 1000; i++)
        {
            server.sendToAll(ping);
        }

        for (auto& client : clients)
            client.update();
        for (auto& client : clients)
            client.disconnect();
        clients.clear();
    }
    serverThread.join();
    return 0;
}