// #include "client.hpp"

// void Client::error(std::string&& errorMessage)
// {
//     disconnect();
//     throw std::runtime_error(errorMessage);
// }

// void Client::connect(const std::string& address, const size_t& port)
// {
//     if (_fd)
//         close(_fd);
//     _fd = ::socket(AF_INET, SOCK_STREAM, 0);
//     if (_fd < 0)
//     {
//         throw std::runtime_error("Cannot create socket");
//     }
//     sockaddr_in sockaddr;
//     sockaddr.sin_family      = AF_INET;
//     sockaddr.sin_addr.s_addr = INADDR_ANY;
//     sockaddr.sin_port        = htons(port);

//     // Convertion de l'adresse IP (Obligatoire)
//     if (inet_pton(AF_INET, address.c_str(), &sockaddr.sin_addr) <= 0)
//         error(_fd, "Adresse Ip invalid !");
//     if (::connect(_fd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0)
//         error(_fd, "Cannot connect to socket !");
// }

// void Client::disconnect()
// {
//     shutdown(_fd, SHUT_RDWR);
//     close(_fd);
//     _fd = 0;
// }

// void Client::defineAction(const Message::Type&                           messageType,
//                           const std::function<void(const Message& msg)>& action)
// {
//     _triggers[messageType].push_back(action);
// }

// void Client::send(const Message& message)
// {
//     int msgType = message.type();
//     ::send(_fd, &msgType, sizeof(msgType), 0);

//     size_t size = message.data().size();
//     ::send(_fd, &size, sizeof(size), 0);

//     if (size > 0)
//     {
//         ::send(_fd, message.data().data(), size, 0);
//     }
// }
// void Client::receiveMessage()
// {
//     int    typeInt;
//     size_t size;

//     int bytes = recv(_fd, &typeInt, sizeof(typeInt), MSG_WAITALL);
//     if (bytes == 0)
//         error(_fd, "Server closed connection");
//     if (bytes == -1)
//         error(_fd, "Cannot receive message");

//     bytes = recv(_fd, &size, sizeof(size), MSG_WAITALL);
//     if (bytes == 0)
//         error(_fd, "Server closed connection");
//     if (bytes == -1)
//         error(_fd, "Cannot receive message");

//     std::vector<unsigned char> msgTab(size);
//     if (size > 0)
//     {
//         bytes = recv(_fd, msgTab.data(), size, MSG_WAITALL);
//         if (bytes == 0)
//             error(_fd, "Server closed connection");
//         if (bytes == -1)
//             error(_fd, "Cannot receive message");
//     }

//     Message msg(typeInt, msgTab);

//     _msgs.push_back(msg);
// }

// void Client::update()
// {
//     while (true)
//     {
//         FD_ZERO(&_readyRead);
//         FD_SET(_fd, &_readyRead);

//         timeval timeout = {0, 0}; // Pour que le select soit non bloquant
//         int     ready   = select(_fd + 1, &_readyRead, NULL, NULL, &timeout);
//         if (ready <= 0 || !FD_ISSET(_fd, &_readyRead))
//             break;
//         receiveMessage();
//     }

//     for (auto& msg : _msgs)
//     {
//         auto it = _triggers.find(msg.type());
//         if (it == _triggers.end()) // Maybe Throw Exception
//             continue;
//         for (auto& funct : it->second)
//             funct(msg);
//     }
//     _msgs.clear();
// }