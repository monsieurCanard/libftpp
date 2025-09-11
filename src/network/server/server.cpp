#include "server.hpp"

void Server::start(const size_t& port)
{
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket < 0)
    {
        throw std::runtime_error("Cannot create socket");
    }

    int opt = 1;
    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        std::cerr << "setsockopt(SO_REUSEADDR) failed: " << strerror(errno) << " (" << errno
                  << ")\n";
        close(_socket);
        return;
    }

    sockaddr_in sockaddr;
    sockaddr.sin_family      = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    sockaddr.sin_port        = htons(port);

    if (bind(_socket, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0)
    {
        stop();
        throw std::runtime_error("Failed to bind on socket. errno: " + std::to_string(errno));
    }

    _max_fd = _socket;
    FD_ZERO(&_active);
    FD_SET(_socket, &_active);

    if (listen(_socket, NB_CONNECTION) < 0)
    {
        stop();
        throw std::runtime_error("Failed to listen on socket. errno: " + std::to_string(errno));
    }

    while (_running)
    {

        _readyRead = _readyWrite = _active;
        timeval timeout          = {0, 10000}; // Pour que le select soit non bloquant
        int     select_result    = select(_max_fd + 1, &_readyRead, &_readyWrite, NULL, &timeout);

        if (select_result < 0)
        {
            if (errno == EBADF || errno == EINTR)
            {
                std::cout << "Select interrupted, stopping server..." << std::endl;
                break;
            }
            continue;
        }

        if (select_result == 0)
            continue;

        for (int fd = 0; fd <= _max_fd; fd++)
        {
            if (!FD_ISSET(fd, &_readyRead))
                continue;
            if (fd == _socket)
            {
                acceptNewConnection();
                continue;
            }
            if (!receiveClientMsg(fd))
            {
                auto clientIt = _clients.find(fd);
                if (clientIt != _clients.end())
                {
                    clearClient(fd, clientIt->second);
                }
                _partialMsgs.erase(fd);
            }
        }
        update();
    }
    stop();
}

void Server::acceptNewConnection()
{
    int connfd = accept(_socket, 0, 0);
    if (connfd < 0)
        return;

    if (_clients.size() >= NB_CONNECTION)
    {
        std::cerr << "Max connections reached, closing new connection" << std::endl;
        close(connfd);
        return;
    }

    if (_max_fd < connfd)
        _max_fd = connfd;

    FD_SET(connfd, &_active);
    _clients[connfd]       = _next_id;
    _clientsToFd[_next_id] = connfd;
    _next_id++;
}

bool Server::receiveClientMsg(const int& fd)
{

    if (!FD_ISSET(fd, &_readyRead))
        return false;

    char buffRead[16000];
    int  bytes = recv(fd, buffRead, sizeof(buffRead), MSG_DONTWAIT);

    if (bytes == 0)
        return false;

    if (bytes == -1)
    {
        // Gérer le cas où il n'y a pas assez de données
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return true; // Pas assez de données, on reviendra plus tard

        return false;
    }

    _partialMsgs[fd].data().pushInto(buffRead, bytes);

    while (_partialMsgs[fd].isComplet())
    {
        Message newMsg;
        newMsg.setType(_partialMsgs[fd].popType());

        auto data = _partialMsgs[fd].popData();
        newMsg.data().pushInto(data.data(), data.size());
        newMsg.setMessageFd(fd);

        _msgs.push_back(newMsg);
        _partialMsgs[fd].reset();
    }
    return true;
}

void Server::sendTo(const Message& message, long long clientID)
{
    try
    {
        auto fdIt = _clientsToFd.find(clientID);
        if (fdIt == _clientsToFd.end())
            return;

        if (!FD_ISSET(fdIt->second, &_active))
            return;

        const auto& data = message.getSerializedData();
        if (data.empty())
            return;

        ssize_t bytes_sent = send(fdIt->second, data.data(), data.size(), 0);
        if (bytes_sent == -1)
        {
            clearClient(fdIt->second, clientID);

            std::cerr << "Failed to send message to client " << clientID << std::endl;
        }
    }
    catch (std::runtime_error& e)
    {
        throw;
    }
}

void Server::sendToArray(const Message& message, std::vector<long long> clientIDs)
{
    for (auto& id : clientIDs)
    {
        sendTo(message, id);
    }
}

void Server::sendToAll(const Message& message)
{

    for (auto& [fd, clientId] : _clients)
    {
        sendTo(message, clientId);
    }
}

void Server::defineAction(
    const Message::Type&                                                messageType,
    const std::function<void(long long& clientID, const Message& msg)>& action)
{
    _tasks[messageType] = action;
}

void Server::update()
{
    for (size_t i = 0; i < _msgs.size(); i++)
    {
        auto it = _tasks.find(_msgs[i].type());

        if (it == _tasks.end())
            continue;

        auto it2 = _clients.find(_msgs[i].getMessageFd());
        if (it2 == _clients.end())
            continue;
        long long clientId = it2->second;
        it->second(clientId, _msgs[i]);
    }
    _msgs.clear();
}

void Server::clearAll()
{
    _clients.clear();
    _clientsToFd.clear();
    _partialMsgs.clear();
    _msgs.clear();
    FD_ZERO(&_active);
}

void Server::clearClient(int& fd, long long& clientId)
{
    if (_clients.find(fd) == _clients.end())
    {
        return;
    }

    if (fd > 2) // Sécurité contre les fd système
    {
        close(fd);
        FD_CLR(fd, &_active);
    }

    _clients.erase(fd);
    _clientsToFd.erase(clientId);
    _partialMsgs.erase(fd);
}

void Server::stop()
{
    _running = false;
    if (_socket >= 0)
    {
        close(_socket);
        _socket = -1;
    }

    for (auto& [fd, clientId] : _clients)
    {
        if (fd <= 2)
        {
            continue;
        }

        close(fd);
        FD_CLR(fd, &_active);
    }

    clearAll();
}