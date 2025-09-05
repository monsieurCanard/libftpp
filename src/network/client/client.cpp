#include "client.hpp"

void Client::error(std::string&& errorMessage)
{
    disconnect();
    throw std::runtime_error(errorMessage);
}

void Client::connect(const std::string& address, const size_t& port)
{
    if (_fd)
        close(_fd);
    _fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (_fd < 0)
    {
        throw std::runtime_error("Cannot create socket");
    }
    sockaddr_in sockaddr;
    sockaddr.sin_family      = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    sockaddr.sin_port        = htons(port);

    // Convertion de l'adresse IP (Obligatoire)
    if (inet_pton(AF_INET, address.c_str(), &sockaddr.sin_addr) <= 0)
        error("Adresse Ip invalid !");
    if (::connect(_fd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0)
        error("Cannot connect to socket !");
}

void Client::disconnect()
{
    shutdown(_fd, SHUT_RDWR);
    close(_fd);
    _fd = 0;
}

void Client::defineAction(const Message::Type&                           messageType,
                          const std::function<void(const Message& msg)>& action)
{
    _triggers[messageType].push_back(action);
}

void Client::send(const Message& message)
{
    auto data = message.getData();
    ::send(_fd, data.data(), data.size(), 0);
}

void Client::receiveMessage()
{
    char buffRead[16000];
    int  bytes = recv(_fd, buffRead, sizeof(buffRead), MSG_DONTWAIT);
    std::cout << " Message recu " << std::endl;
    if (bytes == 0)
    {
        error("Server closed connection");
    }
    if (bytes == -1)
    {
        // Gérer le cas où il n'y a pas assez de données
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return; // Pas assez de données, on reviendra plus tard
        error("Cannot receive message");
    }
    _tmpMsg.data().pushInto(buffRead, bytes);

    while (_tmpMsg.isComplet())
    {
        std::cout << "Message complete" << std::endl;
        Message newMsg(_tmpMsg.getType());
        newMsg.data().push(_tmpMsg.popData());
        _msgs.push_back(newMsg);
        _tmpMsg.reset();
    }
}

void Client::update()
{
    while (_fd > 0)
    {
        FD_ZERO(&_readyRead);
        FD_SET(_fd, &_readyRead);

        timeval timeout = {0, 0}; // Pour que le select soit non bloquant
        int     ready   = select(_fd + 1, &_readyRead, NULL, NULL, &timeout);
        if (ready <= 0 || !FD_ISSET(_fd, &_readyRead))
            break;
        receiveMessage();
    }
    std::cout << "Processing " << _msgs.size() << " messages" << std::endl;
    for (auto& msg : _msgs)
    {
        int type = msg.type();
        std::cout << "message type" << type << std::endl;
        auto it = _triggers.find(type);
        if (it == _triggers.end()) // Maybe Throw Exception
            continue;
        for (auto& funct : it->second)
            funct(msg);
    }
    _msgs.clear();
}