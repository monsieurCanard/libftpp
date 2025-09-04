#ifndef SERVER_HPP
#define SERVER_HPP

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <unordered_map>

#include "network/message.hpp"

#define NB_CONNECTION     128
#define READ_BUFFER_SIZE  4096
#define MAX_CLIENT_BUFFER 32768

#include "message.hpp"
class Server
{
private:
    int _socket;
    int _max_fd;
    int _next_id;

    fd_set _active;
    fd_set _readyRead;
    fd_set _readyWrite;

    std::unordered_map<Message::Type, std::function<void(long long& clientID, const Message& msg)>>
                                       _tasks;
    std::unordered_map<long long, int> _clients;
    std::vector<int, Message>          _clients_buffs;

    void start(const size_t& port);

    void defineAction(const Message::Type& messageType,
                      const std::function<void(long long& clientID, const Message& msg)>& action)
    {
        _tasks[messageType] = action;
    }

    void acceptNewConnection();

    void sendTo(const Message& message, long long clientID)
    {
        send(_clients[clientID], message.data().data(), sizeof(message.data().size()), 0);
    }

    void sendToArray(const Message& message, std::vector<long long> clientIDs)
    {
        for (const long long& id : clientIDs)
        {
            sendTo(message, id);
        }
    }

    void sendToAll(const Message& message)
    {
        for (size_t i = 0; i <= _max_fd; i++)
            sendTo(message, i);
    }
    void update();

public:
};
#endif