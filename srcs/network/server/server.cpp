#include "server.hpp"

void Server::start(const size_t& port)
{
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (socket < 0)
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
        for (int i = 0; i <= _max_fd; i++)
        {
            if (!FD_ISSET(i, &_readyRead))
                continue;
            if (i == _socket)
            {
                acceptNewConnection();
                continue;
            }
            char* buffRead[1024];
            int   byteRead = recv(i, buffRead, sizeof(buffRead) - 1);
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
}