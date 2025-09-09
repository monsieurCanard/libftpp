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

#define MAX_READ_BUFFER 16000

#include "../message/message.hpp"

/*
 * @brief Classe client TCP basique utilisant des sockets POSIX et select()
 * @note Limité par le nombre de byte maximum pouvant être lu d'un coup (MAX_READ_BUFFER)
 *
 * @exception Lance des runtime_error en cas d'erreur
 *
 */
class Client
{
private:
    std::vector<Message> _msgs;
    std::unordered_map<Message::Type, std::vector<std::function<void(const Message& msg)>>>
        _triggers;

    int     _fd = 0;
    fd_set  _readyRead;
    Message _tmpMsg{0};

    void error(std::string&& errorMessage);
    void receiveMessage();

public:
    Client();
    Client(const std::string& address, const size_t& port);

    void connect(const std::string& address, const size_t& port);

    void defineAction(const Message::Type&                           messageType,
                      const std::function<void(const Message& msg)>& action);

    void disconnect();
    void send(const Message& message);
    void update();
};

#endif