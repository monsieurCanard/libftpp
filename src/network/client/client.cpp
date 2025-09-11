#include "client.hpp"

Client::Client() : _tmpMsg(0) {}

Client::Client(const std::string& address, const size_t& port) : _tmpMsg(0)
{
    try
    {
        connect(address, port);
    }
    catch (const std::runtime_error& e)
    {
        throw;
    }
}

void Client::error(std::string&& errorMsg)
{
    disconnect();
    errorMsg += strerror(errno);
    errorMsg += " (errno: " + std::to_string(errno) + ")";
    throw std::runtime_error(errorMsg);
}

void Client::connect(const std::string& address, const size_t& port)
{
    if (_fd)
        close(_fd);

    _fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (_fd < 0)
        return error("Cannot create socket");

    sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port   = htons(port);

    if (inet_pton(AF_INET, address.c_str(), &sockaddr.sin_addr) <= 0)
        return error("Adresse Ip invalid !");

    if (::connect(_fd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0)
        return error("Cannot connect to socket: ");
}

void Client::disconnect()
{
    if (_fd > 0)
    {
        struct linger lg;
        lg.l_onoff  = 1;
        lg.l_linger = 0;
        setsockopt(_fd, SOL_SOCKET, SO_LINGER, &lg, sizeof(lg));

        // shutdown(_fd, SHUT_RDWR);
        close(_fd);
    }
    _fd = 0;
}

void Client::defineAction(const Message::Type&                           messageType,
                          const std::function<void(const Message& msg)>& action)
{
    _triggers[messageType].push_back(action);
}

void Client::send(const Message& message)
{
    try
    {
        auto data = message.getSerializedData();
        ::send(_fd, data.data(), data.size(), 0);
    }
    catch (std::runtime_error& e)
    {
        throw;
    }
}

bool Client::isConnected() const
{
    if (_fd <= 0)
        return false;

    int       error = 0;
    socklen_t len   = sizeof(error);
    return (getsockopt(_fd, SOL_SOCKET, SO_ERROR, &error, &len) == 0 && error == 0);
}

void Client::receiveMessage()
{

    char buffRead[MAX_READ_BUFFER];
    int  bytes = recv(_fd, buffRead, sizeof(buffRead), MSG_DONTWAIT);
    if (bytes == 0)
    {
        disconnect();
        return;
    }
    if (bytes == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return;

        // Déconnexions réseau (ne pas lever d'exception)
        if (errno == ENOTCONN ||   // Transport endpoint is not connected
            errno == ECONNRESET || // Connection reset by peer
            errno == EPIPE ||      // Broken pipe
            errno == EBADF)        // Bad file descriptor
        {
            disconnect();
            return;
        }

        error("Cannot receive message: ");
    }

    try
    {
        _tmpMsg.data().pushInto(buffRead, bytes);
        while (_tmpMsg.isComplet())
        {
            Message newMsg;
            newMsg.setType(_tmpMsg.popType());

            auto data = _tmpMsg.popData();
            newMsg.data().pushInto(data.data(), data.size());

            _msgs.push_back(newMsg);
            _tmpMsg.reset();
        }
    }
    catch (std::runtime_error& e)
    {
        throw;
    }
}

void Client::update()
{
    try
    {
        if (!isConnected())
            return;
        while (_fd > 0)
        {
            FD_ZERO(&_readyRead);
            FD_SET(_fd, &_readyRead);

            timeval timeout = {0, 10000}; // Pour que le select soit non bloquant

            int ready = select(_fd + 1, &_readyRead, NULL, NULL, &timeout);
            if (ready <= 0 || !FD_ISSET(_fd, &_readyRead))
                break;

            receiveMessage();
        }

        for (auto& msg : _msgs)
        {
            auto it = _triggers.find(msg.type());
            if (it == _triggers.end())
                continue;

            for (auto& funct : it->second)
                funct(msg);
        }
        _msgs.clear();
    }
    catch (std::runtime_error& e)
    {
        throw;
    }
}