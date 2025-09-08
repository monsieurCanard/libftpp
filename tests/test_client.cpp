#include <gtest/gtest.h>

#include <chrono>
#include <thread>

#include "libftpp.hpp"

// Petit message factice pour tester
class DummyMessage : public Message
{
public:
    DummyMessage(int t, const std::vector<unsigned char>& d = {}) : Message(t)
    {
        *this << t << d.size() << d;
    }
};

// ---- TESTS UNITAIRES ----

TEST(ClientTest, DefineActionStoresTrigger)
{
    Client c;
    // bool   called = false;
    // c.defineAction(1, [&](const Message& msg) { called = true; });

    // Simule un message
    DummyMessage msg(1);
    c.send(msg); // pas de vrai socket, mais test compilable
    // On "force" le update en poussant le message dans _msgs
    // (hack pour tester sans réseau)
    // normallement on devrait rendre _msgs accessible via friend ou public API

    SUCCEED(); // Le but ici est juste de tester la compilation
}

TEST(ClientTest, ErrorThrowsAndDisconnects)
{
    Client c;
    EXPECT_THROW(c.error("boom"), std::runtime_error);
}

// ---- TESTS D’INTEGRATION AVEC UN SERVEUR FACTICE ----

// Petit serveur TCP local pour tester connect/send/recv
void fakeServer(uint16_t port)
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    ASSERT_NE(server_fd, -1);

    sockaddr_in addr{};
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(port);

    ASSERT_NE(bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)), -1);
    ASSERT_NE(listen(server_fd, 1), -1);

    int client_fd = accept(server_fd, nullptr, nullptr);
    ASSERT_NE(client_fd, -1);

    // Reçoit un int (type)
    int type;
    recv(client_fd, &type, sizeof(type), MSG_WAITALL);

    // Envoie un message de retour
    int                        msgType = 42;
    size_t                     msgSize = 3;
    std::vector<unsigned char> payload = {'a', 'b', 'c'};

    send(client_fd, &msgType, sizeof(msgType), 0);
    send(client_fd, &msgSize, sizeof(msgSize), 0);
    send(client_fd, payload.data(), msgSize, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    close(client_fd);
    close(server_fd);
}

TEST(ClientIntegrationTest, ConnectSendReceive)
{
    uint16_t port = 5555;

    // Lancement du serveur factice dans un thread
    std::thread server([&] { fakeServer(port); });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    Client c;
    ASSERT_NO_THROW(c.connect("127.0.0.1", port));

    // Définition d'une action AVANT l'envoi !
    bool received = false;
    c.defineAction(42,
                   [&](const Message& m)
                   {
                       std::cout << "Action !" << std::endl;
                       received = true;
                       EXPECT_EQ(m.type(), 42);
                   });

    // Envoi d'un message factice
    DummyMessage msg(999);
    c.send(msg);

    // Petit délai pour être sûr que le serveur a répondu
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    // Update (récupère ET traite ce que le serveur a envoyé)
    c.update();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    EXPECT_TRUE(received);

    c.disconnect();
    server.join();
}

// ---- TESTS ADDITIONNELS ----

TEST(ClientTest, ConnectToInvalidAddress)
{
    Client c;
    EXPECT_THROW(c.connect("999.999.999.999", 1234), std::runtime_error);
}

TEST(ClientTest, ConnectToNonExistentPort)
{
    Client c;
    // Port très probablement libre
    EXPECT_THROW(c.connect("127.0.0.1", 65432), std::runtime_error);
}

// TEST(ClientTest, DoubleConnect)
// {
//     // Test que connect() sur un client déjà connecté fonctionne
//     uint16_t port = 5556;

//     std::thread server1([&] { fakeServer(port); });
//     std::this_thread::sleep_for(std::chrono::milliseconds(50));

//     Client c;
//     ASSERT_NO_THROW(c.connect("127.0.0.1", port));

//     c.disconnect();
//     server1.join();

//     // Reconnexion
//     std::thread server2([&] { fakeServer(port); });
//     std::this_thread::sleep_for(std::chrono::milliseconds(50));

//     ASSERT_NO_THROW(c.connect("127.0.0.1", port));
//     c.disconnect();
//     server2.join();
// }

TEST(ClientTest, MultipleActions)
{
    Client c;
    int    callCount = 0;

    // Plusieurs actions pour le même type de message
    c.defineAction(1,
                   [&](const Message& m)
                   {
                       callCount++;
                       m.type();
                   });
    c.defineAction(1,
                   [&](const Message& m)
                   {
                       callCount++;
                       m.type();
                   });
    c.defineAction(1,
                   [&](const Message& m)
                   {
                       callCount++;
                       m.type();
                   });

    // Actions pour différents types
    bool type2Called = false;
    bool type3Called = false;
    c.defineAction(2,
                   [&](const Message& m)
                   {
                       type2Called = true;
                       m.type();
                   });
    c.defineAction(3,
                   [&](const Message& m)
                   {
                       type3Called = true;
                       m.type();
                   });

    // Simule des messages (nécessite un hack pour tester sans réseau)
    SUCCEED(); // Pour l'instant, juste vérifier que ça compile
}

TEST(ClientTest, UpdateWithoutConnection)
{
    Client c;
    // update() sur un client non connecté ne doit pas planter
    EXPECT_NO_THROW(c.update());
}

TEST(ClientTest, DisconnectWithoutConnection)
{
    Client c;
    // disconnect() sur un client non connecté ne doit pas planter
    EXPECT_NO_THROW(c.disconnect());
}

// Serveur qui envoie plusieurs messages
void multiMessageServer(uint16_t port)
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    ASSERT_NE(server_fd, -1);

    sockaddr_in addr{};
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(port);

    ASSERT_NE(bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)), -1);
    ASSERT_NE(listen(server_fd, 1), -1);

    int client_fd = accept(server_fd, nullptr, nullptr);
    ASSERT_NE(client_fd, -1);

    // Reçoit un message du client
    int type;
    recv(client_fd, &type, sizeof(type), MSG_WAITALL);

    // Envoie 3 messages différents
    for (int i = 1; i <= 3; i++)
    {
        std::vector<unsigned char> payload = {'A', (unsigned char)('0' + i)};
        DummyMessage               msg(100 + i, payload); // Utilise DummyMessage !

        auto data = msg.getData();
        send(client_fd, data.data(), data.size(), 0);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    close(client_fd);
    close(server_fd);
}

TEST(ClientIntegrationTest, ReceiveMultipleMessages)
{
    uint16_t port = 5557;

    std::thread server([&] { multiMessageServer(port); });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    Client c;
    ASSERT_NO_THROW(c.connect("127.0.0.1", port));

    int msg101Count = 0, msg102Count = 0, msg103Count = 0;

    c.defineAction(101,
                   [&](const Message& m)
                   {
                       msg101Count++;
                       EXPECT_EQ(m.type(), 101);
                   });
    c.defineAction(102,
                   [&](const Message& m)
                   {
                       msg102Count++;
                       EXPECT_EQ(m.type(), 102);
                   });
    c.defineAction(103,
                   [&](const Message& m)
                   {
                       msg103Count++;
                       EXPECT_EQ(m.type(), 103);
                   });

    // Envoi d'un message pour déclencher la réponse du serveur
    DummyMessage msg(999);
    c.send(msg);

    for (int i = 0; i < 5; i++)
    {
        c.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    EXPECT_EQ(msg101Count, 1);
    EXPECT_EQ(msg102Count, 1);
    EXPECT_EQ(msg103Count, 1);

    c.disconnect();
    server.join();
}

// Serveur qui ferme brutalement la connexion
void brutaldisconnectServer(uint16_t port)
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    ASSERT_NE(server_fd, -1);

    sockaddr_in addr{};
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(port);

    ASSERT_NE(bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)), -1);
    ASSERT_NE(listen(server_fd, 1), -1);

    int client_fd = accept(server_fd, nullptr, nullptr);
    ASSERT_NE(client_fd, -1);

    // Ferme immédiatement sans répondre
    close(client_fd);
    close(server_fd);
}

TEST(ClientIntegrationTest, HandleServerDisconnect)
{
    uint16_t port = 5558;

    std::thread server([&] { brutaldisconnectServer(port); });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    Client c;
    ASSERT_NO_THROW(c.connect("127.0.0.1", port));

    DummyMessage msg(999);
    c.send(msg);

    EXPECT_NO_THROW(c.update());

    // update() doit gérer la déconnexion sans planter
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    c.disconnect();
    server.join();
}

// Serveur qui envoie des données partielles
void partialDataServer(uint16_t port)
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    ASSERT_NE(server_fd, -1);

    sockaddr_in addr{};
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(port);

    ASSERT_NE(bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)), -1);
    ASSERT_NE(listen(server_fd, 1), -1);

    int client_fd = accept(server_fd, nullptr, nullptr);
    ASSERT_NE(client_fd, -1);

    // Reçoit du client
    int type;
    recv(client_fd, &type, sizeof(type), MSG_WAITALL);

    // Envoie le type
    int msgType = 200;
    send(client_fd, &msgType, sizeof(msgType), 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // Envoie la taille
    size_t msgSize = 5;
    send(client_fd, &msgSize, sizeof(msgSize), 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // Envoie les données en 2 fois
    std::vector<unsigned char> payload = {'H', 'E', 'L', 'L', 'O'};
    send(client_fd, payload.data(), 3, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    send(client_fd, payload.data() + 3, 2, 0);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    close(client_fd);
    close(server_fd);
}

TEST(ClientIntegrationTest, HandlePartialData)
{
    uint16_t port = 5559;

    std::thread server([&] { partialDataServer(port); });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    Client c;
    ASSERT_NO_THROW(c.connect("127.0.0.1", port));

    bool received = false;
    c.defineAction(200,
                   [&](const Message& m)
                   {
                       received = true;
                       EXPECT_EQ(m.type(), 200);
                   });

    DummyMessage msg(200);
    c.send(msg);

    for (int i = 0; i < 10; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        c.update();
        // std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (received)
            break;
    }

    EXPECT_TRUE(received);

    c.disconnect();
    server.join();
}
