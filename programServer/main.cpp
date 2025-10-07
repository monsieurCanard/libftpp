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
                               std::cout << "[CHUCK NORRIS]" << std::endl;
                               double Tvalue = 0;
                               std::cout << Tvalue << std::endl;

                               msg.insertValue(Tvalue);
                               std::cout << Tvalue << std::endl;
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

        saver.defineAction(7,
                           [&fs](const Message& msg) { fs << msg.messageToString() << std::endl; });

        saver.defineAction(1,
                           [&](const Message& msg)
                           {
                               (void)msg;
                               std::cout << "[CHUCK NORRIS]" << std::endl;
                               std::cout << "pong" << std::endl;
                           });

        Client antoine;
        antoine.defineAction(8,
                             [&](const Message& msg)
                             {
                                 std::cout << "[ANTOINE DANIEL]" << std::endl;
                                 std::string value = msg.messageToString();
                                 std::cout << value << std::endl;
                                 printAntoine();
                             });
        antoine.defineAction(1,
                             [&](const Message& msg)
                             {
                                 (void)msg;
                                 std::cout << "[ANTOINE DANIEL]" << std::endl;
                                 std::cout << "pong" << std::endl;
                             });

        chuck.connect(address, port);
        antoine.connect(address, port);
        saver.connect(address, port);
        clients.insert(clients.end(), {chuck, antoine, saver});

        std::map<std::string, int> triggers = {
            {"stop", 0}, {"ping", 1}, {"save", 2}, {"antoine", 8}, {"chuck", 7}};

        while (1)
        {
            std::string input;
            std::cin >> input;
            std::cin.clear();

            for (auto& letter : input)
                letter = tolower(letter);

            Message sender(7);
            sender.setType(triggers.contains(input) ? triggers[input] : 7);

            if (sender.type() == 0)
                break;

            server.sendToAll(sender);

            // Une premier idee de logger dans un client
            Message loggerMessage(2);
            loggerMessage << "Message type " + std::to_string(sender.type()) + " sent to clients";
            server.sendToAll(loggerMessage);

            // Mon logger code dans la librairy
            logger.log(LogLevel::INFO,
                       "Message type " + std::to_string(sender.type()) + " sent to clients");

            for (auto& client : clients)
                client.update();
        }

        for (auto& client : clients)
            client.disconnect();

        server.stop();
        if (thread.joinable())
        {
            thread.join(); // Permet au thread de continuer en arrière-plan
        }
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}