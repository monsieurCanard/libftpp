#include <unistd.h>

#include <chrono>
#include <exception>
#include <fstream>
#include <thread>
#include <vector>

#include "../libftpp.hpp"
#include "chuck.hpp"

void printChuckNorris(Chuck& chuck)
{
    std::cout << chuck.getRandomQuote() << std::endl;
}

void printAntoine()
{
    std::cout << "Je suis antoine daniel" << std::endl;
}

void update_client_logger(Server& server, const Message& msg)
{
    Message loggerMessage(2);
    loggerMessage << "Server send message type: " + msg.messageToString();
    server.sendToAll(loggerMessage);
}

void clean_exit(std::vector<Client>& clients, Server& server, std::thread& thread)
{
    std::cout << "Server is shutting down, disconnecting clients..." << std::endl;
    server.stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << "All clients disconnected." << std::endl;

    if (thread.joinable())
        thread.join();

    for (auto& client : clients)
        client.disconnect();
    std::cout << "Server thread joined." << std::endl;
    return;
}

int main()
{
    Server              server;
    std::string         address = "127.0.0.1";
    int                 port    = 7779;
    Chuck               chuckSpeaker;
    Logger              logger("server.log", LogLevel::DEBUG);
    std::vector<Client> clients;

    /**
     * ! Rajouter si on veux laisser le choix du port et de l'adresse
     */
    // std::cout << "Choose your port :" << std::endl;
    // std::cin >> port;
    // std::cout << "Choose your adress :" << std::endl;
    // std::cin >> address;
    // std::cin.clear();
    std::thread thread(&Server::start, &server, port);

    try
    {

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        Client chuck;
        chuck.defineAction(7,
                           [&chuckSpeaker](const Message& msg)
                           {
                               (void)msg;
                               std::cout << "[CHUCK NORRIS]" << std::endl;
                               //    double Tvalue = 0;
                               //    std::cout << Tvalue << std::endl;

                               //    msg.insertValue(Tvalue);
                               //    std::cout << Tvalue << std::endl;
                               printChuckNorris(chuckSpeaker);
                           });
        chuck.defineAction(1,
                           [&](const Message& msg)
                           {
                               (void)msg;
                               std::cout << "[CHUCK NORRIS]" << std::endl;
                               std::cout << "pong" << std::endl;
                           });

        Client       saver;
        std::fstream fs("save.txt", std::ios::out | std::ios::trunc);
        if (!fs.is_open())
            throw std::runtime_error("Could not open save.txt");

        saver.defineAction(2,
                           [&fs](const Message& msg) { fs << msg.messageToString() << std::endl; });

        saver.defineAction(1,
                           [&](const Message& msg)
                           {
                               (void)msg;
                               std::cout << "[SAVER]" << std::endl;
                               std::cout << "pong" << std::endl;
                           });

        Client antoine;
        antoine.defineAction(8,
                             [&](const Message& msg)
                             {
                                 (void)msg;
                                 std::cout << "[ANTOINE DANIEL]" << std::endl;
                                 printAntoine();
                             });
        antoine.defineAction(1,
                             [&](const Message& msg)
                             {
                                 (void)msg;
                                 std::cout << "[ANTOINE DANIEL]" << std::endl;
                                 std::cout << "pong" << std::endl;
                             });

        clients.insert(clients.end(), {chuck, antoine, saver});

        for (auto& client : clients)
            client.connect(address, port);

        std::map<std::string, int> triggers = {
            {"stop", 0}, {"ping", 1}, {"save", 2}, {"antoine", 8}, {"chuck", 7}};

        while (1)
        {
            std::string input;
            std::cin >> input;

            for (auto& letter : input)
                letter = tolower(letter);

            Message sender(7);

            auto it = triggers.find(input);
            if (it != triggers.end())
                sender.setType(it->second);

            if (sender.type() == 0)
            {
                std::cout << "Stopping server..." << std::endl;
                clean_exit(clients, server, thread);
                return 0;
            }

            sender << input;
            server.sendToAll(sender);

            for (auto& client : clients)
                client.update();

            update_client_logger(server, sender);
            // Une premier idee de logger dans un client
            for (auto& client : clients)
                client.update();
            // // Mon logger code dans la librairy
            logger.log(LogLevel::INFO,
                       "Message type " + std::to_string(sender.type()) + " sent to clients");
        }
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}