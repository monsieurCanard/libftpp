#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstddef>
#include <functional>
#include <string>
#include <unordered_map>

#include "../message/message.hpp"

class Client
{
private:
    std::vector<Message> _msgs;
    std::unordered_map<Message::Type, std::vector<std::function<void(const Message& msg)>>>
        _triggers;

    int     _fd = 0;
    fd_set  _readyRead;
    Message _tmpMsg;

public:
    Client() : _tmpMsg(0) {}
    void error(const std::string& errorMessage);

    void connect(const std::string& address, const size_t& port);

    void disconnect();

    void defineAction(const Message::Type&                           messageType,
                      const std::function<void(const Message& msg)>& action);
    void send(const Message& message);
    void receiveMessage();
    void update();
};

#endif