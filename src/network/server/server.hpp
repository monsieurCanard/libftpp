#ifndef SERVER_HPP
#define SERVER_HPP

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <unordered_map>

#define NB_CONNECTION    1000
#define READ_BUFFER_SIZE 100000
// 4096

#include "../message/message.hpp"

/**
 * @brief Classe serveur TCP basique utilisant des sockets POSIX et select()
 * @note Gère les connexions, déconnexions, envois et réceptions de messages
 * @note Utilise la classe Message pour le format des messages
 * @note Permet de définir des actions (callbacks) pour différents types de messages
 * @note Non thread-safe, doit être utilisé dans un seul thread (généralement le thread principal)
 * @note Limité par le nombre de connections simultanées (NB_CONNECTION)
 * @note Limité par le nombre de byte maximum pouvant être lu d'un coup (READ_BUFFER_SIZE)
 *
 * @exception Lance des runtime_error en cas d'erreur
 *
 */
class Server
{
private:
    int         _socket  = -1;
    int         _max_fd  = -1;
    int         _next_id = 0;
    bool        _running = true;
    std::string _address;
    size_t      _port;

    fd_set _active;
    fd_set _readyRead;
    fd_set _readyWrite;

    std::unordered_map<int, long long> _clients;
    std::unordered_map<long long, int> _clientsToFd;
    std::unordered_map<Message::Type, std::function<void(long long& clientID, const Message& msg)>>
        _tasks;

    std::vector<Message>             _msgs;
    std::unordered_map<int, Message> _partialMsgs;

    void _acceptNewConnection();
    bool _receiveClientMsg(const int& fd);

    void _clearAll();
    void _clearClient(int& fd);

public:
    Server();
    Server(const std::string& address, size_t port);
    void start(const size_t& port = 0);

    void defineAction(const Message::Type& messageType,
                      const std::function<void(long long& clientID, const Message& msg)>& action);

    void sendTo(const Message& message, long long clientID);
    void sendToArray(const Message& message, std::vector<long long> clientIDs);
    void sendToAll(const Message& message);

    void update();
    void stop();
};
#endif