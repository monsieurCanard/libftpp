#include "server.hpp"

void Server::stop()
{
    for (auto& pair : _clients)
    {
        close(pair.second);
        FD_CLR(pair.second, &_active);
    }
}

void Server::start(const size_t& port)
{
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket <= 0)
    {
        throw std::runtime_error("Cannot create socket");
    }
    sockaddr_in sockaddr;
    sockaddr.sin_family      = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    sockaddr.sin_port        = htons(port);
    if (bind(_socket, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0)
        throw std::runtime_error("Failed to bind on socket. errno: " + errno);

    _max_fd = _socket;
    FD_ZERO(&_active);
    FD_SET(_socket, &_active);

    if (listen(_socket, NB_CONNECTION) < 0)
        throw std::runtime_error("Failed to listen on socket. errno: " + errno);
    while (1)
    {
        _readyRead = _readyWrite = _active;
        if (select(_max_fd + 1, &_readyRead, &_readyWrite, NULL, NULL) < 0)
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
                close(fd);
                FD_CLR(fd, &_active);
            }
        }
    }
}

void Server::acceptNewConnection()
{
    int connfd = accept(_socket, 0, 0);
    if (connfd < 0)
        return;
    if (_max_fd < connfd)
        _max_fd = connfd;
    FD_SET(connfd, &_active);
    _clients[_next_id++] = connfd;
}

bool Server::receiveClientMsg(const int& fd)
{
    if (!FD_ISSET(fd, &_readyRead))
        return false; // Maybe throw something
    char buffRead[16000];
    int  bytes = recv(fd, buffRead, sizeof(buffRead), MSG_DONTWAIT);
    std::cout << " Message recu " << std::endl;
    if (bytes == 0)
    {
        std::cout << "Client closed connection" << std::endl;
        // disconnect();
        return false;
    }
    if (bytes == -1)
    {
        // Gérer le cas où il n'y a pas assez de données
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return true; // Pas assez de données, on reviendra plus tard
                         // error("Cannot receive message");
        return false;
    }
    _partialMsgs[fd].data().pushInto(buffRead, bytes);

    while (_partialMsgs[fd].isComplet())
    {
        std::cout << "Message complete" << std::endl;
        Message newMsg(_partialMsgs[fd].type());
        newMsg.data().push(_partialMsgs[fd].popData());
        _msgs.push_back(newMsg);
        _partialMsgs[fd].reset();
    }
    return true;
}

void Server::sendTo(const Message& message, long long clientID)
{
    const auto& data = message.getData();
    if (data.empty())
        return;

    ssize_t bytes_sent = send(_clients[clientID], data.data(), data.size(), 0);
    if (bytes_sent == -1)
    {
        // Gérer l'erreur d'envoi
        std::cerr << "Failed to send message to client " << clientID << std::endl;
    }
}

void Server::sendToArray(const Message& message, std::vector<long long> clientIDs)
{
    for (auto& id : clientIDs)
    {
        if (FD_ISSET(_clients[id], &_active))
        {
            sendTo(message, id);
        }
    }
}

void Server::sendToAll(const Message& message)
{
    for (auto& [id, fd] : _clients)
    {
        if (FD_ISSET(fd, &_active))
        {
            sendTo(message, id);
        }
    }
}

void Server::defineAction(
    const Message::Type&                                                messageType,
    const std::function<void(long long& clientID, const Message& msg)>& action)
{
    _tasks[messageType] = action;
}