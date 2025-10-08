#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <arpa/inet.h>
#include <netdb.h>
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

/**
 * @brief Classe client TCP basique utilisant des sockets POSIX et select()
 * @note Limité par le nombre de byte maximum pouvant être lu d'un coup (MAX_READ_BUFFER)
 *
 * @throw Lance des std::out_of_range en cas d'erreur de lecture dans un message
 * @throw Lance des std::runtime_error en cas d'erreur réseau
 * @throw Relance toutes les exceptions lancées par les callbacks
 *
 */
class Client
{
private:
    std::vector<Message> _msgs;
    std::unordered_map<Message::Type, std::vector<std::function<void(const Message& msg)>>>
        _triggers;

    Message _tmpMsg;
    int     _fd;

    fd_set _readyRead;

    void _networkError(std::string&& errorMessage);
    void _receiveMessage();
    bool _isConnected() const;

public:
    Client();
    Client(const std::string& address, const size_t& port);

    void connect(const std::string& address, const size_t& port);
    void disconnect();

    void defineAction(const Message::Type&                           messageType,
                      const std::function<void(const Message& msg)>& action);

    void send(const Message& message);
    void update();
};

#endif