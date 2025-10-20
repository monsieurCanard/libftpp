#include <algorithm>
#include <string>

#include "../libftpp.hpp"

int main()
{
    Client client;

    client.defineAction(3,
                        [](const Message& msg)
                        {
                            int doubledValue;
                            msg >> doubledValue;
                            threadSafeCout << "Received a doubled value: " << doubledValue
                                           << std::endl;
                        });

    // Connect to the server
    client.connect("localhost", 8080);

    Client client2;

    client2.defineAction(3,
                         [](const Message& msg)
                         {
                             int doubledValue;
                             msg >> doubledValue;
                             threadSafeCout << "Received a doubled value: " << doubledValue
                                            << std::endl;
                         });

    // Connect to the server
    client2.connect("localhost", 8080);

    Message     message1(2);
    std::string str = "Good night";
    message1 << str.size();
    threadSafeCout << " str.size() = " << str.size() << std::endl;
    for (char c : str)
    {
        message1 << c;
    }
    client2.send(message1);

    Message message2(3);
    message2 << 5.2;
    client2.send(message2);

    Message message3(1);
    message3 << 42;
    client.send(message3);

    Message     message4(2);
    std::string str2 = "Hello";
    message4 << str2.size();
    threadSafeCout << " str2.size() = " << str2.size() << std::endl;
    for (char c : str2)
    {
        message4 << c;
    }
    client.send(message4);

    Message message5(3);
    message5 << 3.14;
    client.send(message5);

    bool quit = false;

    while (!quit)
    {
        client.update();

        threadSafeCout << "Client updated." << std::endl;
        threadSafeCout << "Available operations :" << std::endl;
        threadSafeCout << " - [Q]uit : close the program" << std::endl;
        threadSafeCout << " - Any other input to continue updating the client" << std::endl;

        std::string input;
        std::getline(std::cin, input);

        std::transform(input.begin(),
                       input.end(),
                       input.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        if (input == "quit" || (input.length() == 1 && input[0] == 'q'))
        {
            quit = true;
        }
    }

    // Disconnect from the server
    client.disconnect();

    return 0;
}
