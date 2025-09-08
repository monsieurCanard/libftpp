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

#define NB_CONNECTION     128
#define READ_BUFFER_SIZE  4096
#define MAX_CLIENT_BUFFER 32768

#include "libftpp.hpp"
class Server
{
private:
    int  _socket;
    int  _max_fd;
    int  _next_id;
    bool _running = true;

    fd_set _active;
    fd_set _readyRead;
    fd_set _readyWrite;

    std::unordered_map<Message::Type, std::function<void(long long& clientID, const Message& msg)>>
                                       _tasks;
    std::unordered_map<int, long long> _clients;
    std::vector<Message>               _msgs;
    std::unordered_map<int, Message>   _partialMsgs;

    void acceptNewConnection();
    bool receiveClientMsg(const int& fd);

public:
    void start(const size_t& port);

    void defineAction(const Message::Type& messageType,
                      const std::function<void(long long& clientID, const Message& msg)>& action);

    void update();
    void stop();
    void sendTo(const Message& message, long long clientID);
    void sendToArray(const Message& message, std::vector<long long> clientIDs);
    void sendToAll(const Message& message);
};
#endif