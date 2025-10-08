#include "libftpp.hpp"

int main(void)
{
    Server      server;
    std::thread serverThread(&Server::start, &server, 8001);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::vector<Client> clients;
    Message             ping(1);
    ping << "ping";
    for (int j = 0; j < 5; j++)
    {
        for (int i = 0; i < 150; i++)
        {
            clients.emplace_back("127.0.0.1", 8001);
            // clients.back().connect("127.0.0.1", 9000);
            clients.back().defineAction(1,
                                        [](const Message& msg)
                                        {
                                            std::string value;
                                            value = msg.messageToString();
                                            std::cout << "Received: " << value << std::endl;
                                        });

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
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
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "---- Cycle " << j + 1 << " complete ----" << std::endl;
    }
    server.stop();
    if (serverThread.joinable())
    {
        serverThread.join();
    }
    return 0;
}