#include <chrono>
#include <exception>
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
    Server      server;
    std::string address = "127.0.0.1";
    int         port    = 7779;
    Chuck       chuckSpeaker;

    /**
     * ! Rajouter si on veux laisser le choix du port et de l'adresse
     */
    // std::cout << "Choose your port :" << std::endl;
    // std::cin >> port;
    // std::cout << "Choose your adress :" << std::endl;
    // std::cin >> address;
    // std::cin.clear();

    try
    {
        std::thread thread(&Server::start, &server, port);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        Client chuck;
        chuck.defineAction(7,
                           [&chuckSpeaker](const Message& msg)
                           {
                               std::cout << "[CHUCK NORRIS]" << std::endl;

                               //    std::string value = msg.messageToString();
                               //    if (value.empty())
                               //    {
                               double Tvalue = 0;
                               std::cout << Tvalue << std::endl;

                               msg.insertValue(Tvalue);
                               std::cout << Tvalue << std::endl;
                               //    }
                               //    else
                               //    {
                               //        std::cout << value << std::endl;
                               //    }
                               printChuckNorris(chuckSpeaker);
                           });
        chuck.defineAction(1,
                           [&](const Message& msg)
                           {
                               (void)msg;
                               std::cout << "[CHUCK NORRIS]" << std::endl;
                               std::cout << "pong" << std::endl;
                           });

        chuck.connect(address, port);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

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

        antoine.connect(address, port);

        // Tellement rapide que pour l'instant pas besoin !
        // std::this_thread::sleep_for(std::chrono::milliseconds(100));
        while (1)
        {
            std::string input;
            std::cin >> input;
            for (auto& letter : input)
                letter = tolower(letter);
            if (input == "stop")
            {
                server.stop();
                break;
            }

            Message sender(7);
            if (input == "antoine")
                sender.setType(8);
            else if (input == "ping")
                sender.setType(1);
            sender << 3.1444444444;
            server.sendToAll(sender);

            chuck.update();
            antoine.update();
        }

        if (thread.joinable())
        {
            thread.join(); // Permet au thread de continuer en arrière-plan
        }
    }
    catch (...)
    {
        std::cout << "e.what()" << std::endl;
        return 1;
    }
    return 0;
}