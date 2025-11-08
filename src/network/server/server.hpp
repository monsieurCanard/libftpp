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
#define READ_BUFFER_SIZE 4096

#include "../message/message.hpp"

/**
 * @brief Basic TCP server class using POSIX sockets and select() for multi-client communication.
 *
 * This class provides a robust TCP server implementation that can handle multiple simultaneous
 * client connections using select() for non-blocking I/O operations. It supports message-based
 * communication with callback functions for different message types.
 *
 * @note Handles connections, disconnections, sending and receiving messages automatically
 * @note Uses Message class for structured message format and parsing
 * @note Limited by maximum simultaneous connections (NB_CONNECTION = 1000)
 * @note Limited by maximum bytes that can be read at once (READ_BUFFER_SIZE = 100000)
 *
 * @code
 * // Create and start server
 * Server server("127.0.0.1", 8080);
 * server.start();
 *
 * // Define message handler for specific message type
 * server.defineAction(1001, [](long long& clientID, const Message& msg) {
 *     std::string request;
 *     msg >> request;
 *     std::cout << "Client " << clientID << " says: " << request << std::endl;
 *
 *     // Send response back to client
 *     Message response(1002);
 *     response << std::string("Hello Client " + std::to_string(clientID));
 *     server.sendTo(response, clientID);
 * });
 *
 * // Main server loop
 * while (running) {
 *     server.update(); // Process incoming connections and messages
 * }
 *
 * server.stop();
 * @endcode
 *
 * @throws std::runtime_error on network errors (bind, listen, accept failures)
 * @see Message for message format and usage
 * @see Client for corresponding client implementation
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
    ~Server();
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