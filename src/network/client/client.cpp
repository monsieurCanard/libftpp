#include "client.hpp"

Client::Client() : _tmpMsg(), _fd(-1) {}

Client::Client(const std::string& address, const size_t& port) : _tmpMsg(), _fd(-1)
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

void Client::_networkError(std::string&& errorMsg)
{
    disconnect();
    errorMsg += strerror(errno);
    errorMsg += " (errno: " + std::to_string(errno) + ")";
    throw std::runtime_error(errorMsg);
}

void Client::connect(const std::string& address, const size_t& port)
{
    if (_fd)
        disconnect();

    _fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (_fd < 0)
        return _networkError("Cannot create socket");

    sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port   = htons(port);

    if (inet_pton(AF_INET, address.c_str(), &sockaddr.sin_addr) <= 0)
        return _networkError("Adresse Ip invalid !");

    if (::connect(_fd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0)
        return _networkError("Cannot connect to socket: ");
}

void Client::disconnect()
{
    if (_fd > 0)
    {
        shutdown(_fd, SHUT_RDWR);
        close(_fd);
    }
    _fd = -1;
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
    catch (std::out_of_range& e)
    {
        throw;
    }
}

bool Client::_isConnected() const
{
    if (_fd <= 0)
        return false;

    int       error = 0;
    socklen_t len   = sizeof(error);

    // Si la socket est fermée, getsockopt va échouer ou renvoyer une erreur
    return (getsockopt(_fd, SOL_SOCKET, SO_ERROR, &error, &len) == 0 && error == 0);
}

void Client::_receiveMessage()
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
        // Pas de données à lire (ne pas lever d'exception)
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

        _networkError("Cannot receive message: ");
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
    catch (std::out_of_range& e)
    {
        throw;
    }
}

void Client::update()
{
    try
    {
        if (!_isConnected())
            return;
        while (_fd > 0)
        {
            FD_ZERO(&_readyRead);
            FD_SET(_fd, &_readyRead);

            timeval timeout = {0, 10000}; // Pour que le select soit non bloquant

            int ready = select(_fd + 1, &_readyRead, NULL, NULL, &timeout);
            if (ready <= 0 || !FD_ISSET(_fd, &_readyRead))
                break;

            _receiveMessage();
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
    catch (std::exception& e)
    {
        throw;
    }
}