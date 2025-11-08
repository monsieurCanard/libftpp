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
 * @brief Basic TCP client class using POSIX sockets and select() for network communication.
 *
 * This class provides a simple TCP client implementation with message-based communication.
 * It supports asynchronous message handling through callback functions and maintains
 * connection state using file descriptors and select() for non-blocking operations.
 *
 * @note Limited by the maximum number of bytes that can be read at once (MAX_READ_BUFFER = 16000)
 * @note Uses select() for non-blocking socket operations
 * @note Supports callback-based message handling with type-specific actions
 * @note Automatically handles message parsing and reconstruction for partial reads
 *
 * @code
 * // Create and connect to server
 * Client client;
 * client.connect("127.0.0.1", 8080);
 *
 * // Define message handler for specific message type
 * client.defineAction(1001, [](const Message& msg) {
 *     std::string response;
 *     msg >> response;
 *     std::cout << "Received: " << response << std::endl;
 * });
 *
 * // Send a message
 * Message msg(1001);
 * msg << std::string("Hello Server");
 * client.send(msg);
 *
 * // Process incoming messages
 * client.update(); // Call regularly in your main loop
 *
 * client.disconnect();
 * @endcode
 *
 * @throws std::out_of_range on message parsing errors
 * @throws std::runtime_error on network connection errors
 * @throws May re-throw any exceptions from registered callback functions
 * @see Message for message format and usage
 * @see Server for corresponding server implementation
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