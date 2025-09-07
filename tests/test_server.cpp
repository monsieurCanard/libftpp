#include <arpa/inet.h>
// #include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <chrono>
#include <thread>

#include "../src/network/client/client.hpp"
#include "../src/network/message/message.hpp"
#include "../src/network/server/server.hpp"

class ServerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        server   = std::make_unique<Server>();
        client   = std::make_unique<Client>();
        testPort = 8080;
    }

    void TearDown() override
    {
        if (server)
        {
            server->stop();
        }
        if (client)
        {
            client->disconnect();
        }
    }

    std::unique_ptr<Server> server;
    std::unique_ptr<Client> client;
    size_t                  testPort;
};

// // Tests pour la classe Message
// class MessageTest : public ::testing::Test
// {
// protected:
//     void SetUp() override
//     {
//         message = std::make_unique<Message>(1);
//     }

//     std::unique_ptr<Message> message;
// };

// TEST_F(MessageTest, MessageCreation)
// {
//     EXPECT_EQ(message->type(), 1);
//     EXPECT_EQ(message->getType(), 1);
// }

// TEST_F(MessageTest, MessageIntOperators)
// {
//     int testValue = 42;
//     *message << testValue;

//     int retrievedValue;
//     *message >> retrievedValue;

//     EXPECT_EQ(retrievedValue, testValue);
// }

// TEST_F(MessageTest, MessageStringOperators)
// {
//     std::string testString = "Hello, World!";
//     *message << testString;

//     std::string retrievedString;
//     *message >> retrievedString;

//     EXPECT_EQ(retrievedString, testString);
// }

// TEST_F(MessageTest, MessageDoubleOperators)
// {
//     double testValue = 3.14159;
//     *message << testValue;

//     double retrievedValue;
//     *message >> retrievedValue;

//     EXPECT_DOUBLE_EQ(retrievedValue, testValue);
// }

// TEST_F(MessageTest, MessageReset)
// {
//     int testValue = 123;
//     *message << testValue;

//     message->reset();

//     // Après reset, le buffer devrait être vide
//     EXPECT_TRUE(message->data().isEmpty());
// }

// TEST_F(MessageTest, MessageTypeConstMethod)
// {
//     const Message constMessage(5);
//     EXPECT_EQ(constMessage.type(), 5);
// }

// // Tests pour la classe Client
// class ClientTest : public ::testing::Test
// {
// protected:
//     void SetUp() override
//     {
//         client   = std::make_unique<Client>();
//         testPort = 8081;
//     }

//     void TearDown() override
//     {
//         if (client)
//         {
//             client->disconnect();
//         }
//     }

//     std::unique_ptr<Client> client;
//     size_t                  testPort;
// };

// TEST_F(ClientTest, ClientCreation)
// {
//     EXPECT_NO_THROW(Client testClient);
// }

// TEST_F(ClientTest, ClientConnectionToInvalidAddress)
// {
//     EXPECT_THROW(client->connect("invalid.address", testPort), std::runtime_error);
// }

// TEST_F(ClientTest, ClientDefineAction)
// {
//     bool          actionCalled = false;
//     Message::Type testType     = 1;

//     auto action = [&actionCalled](const Message& msg)
//     {
//         msg.getData();
//         actionCalled = true;
//     };

//     EXPECT_NO_THROW(client->defineAction(testType, action));
// }

// Tests pour le serveur avec des fonctionnalités de base
TEST_F(ServerTest, ServerCreationAndDestruction)
{
    EXPECT_NO_THROW(Server testServer);
}

TEST_F(ServerTest, ServerDefineAction)
{
    bool          actionCalled = false;
    Message::Type testType     = 1;

    auto action = [&actionCalled](long long& clientID, const Message& msg)
    {
        std::cout << clientID << std::endl;
        msg.getData();
        actionCalled = true;
    };

    EXPECT_NO_THROW(server->defineAction(testType, action));
}

TEST_F(ServerTest, ServerStartWithInvalidPort)
{
    // Port 0 est généralement invalide pour les serveurs
    EXPECT_THROW(server->start(0), std::runtime_error);
}

// Tests d'intégration Client-Server
class ClientServerIntegrationTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        server       = std::make_unique<Server>();
        client       = std::make_unique<Client>();
        testPort     = 8082;
        serverThread = nullptr;
    }

    void TearDown() override
    {
        if (serverThread && serverThread->joinable())
        {
            server->stop();
            serverThread->join();
        }
        if (client)
        {
            client->disconnect();
        }
    }

    void StartServerInThread()
    {
        serverThread = std::make_unique<std::thread>(
            [this]()
            {
                try
                {
                    server->start(testPort);
                }
                catch (const std::exception& e)
                {
                    // Server stopped normally
                }
            });

        // Petite pause pour laisser le serveur démarrer
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::unique_ptr<Server>      server;
    std::unique_ptr<Client>      client;
    std::unique_ptr<std::thread> serverThread;
    size_t                       testPort;
};

TEST_F(ClientServerIntegrationTest, ServerStartsSuccessfully)
{
    EXPECT_NO_THROW(StartServerInThread());
}

TEST_F(ClientServerIntegrationTest, ClientCanConnectToServer)
{
    StartServerInThread();

    // Tentative de connexion du client
    EXPECT_NO_THROW(client->connect("127.0.0.1", testPort));
}

// Tests de stress et edge cases
class ServerStressTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        server   = std::make_unique<Server>();
        testPort = 8083;
    }

    void TearDown() override
    {
        if (server)
        {
            server->stop();
        }
    }

    std::unique_ptr<Server> server;
    size_t                  testPort;
};

TEST_F(ServerStressTest, ServerHandlesMultipleActionDefinitions)
{
    for (int i = 0; i < 100; ++i)
    {
        auto action = [i](long long& clientID, const Message& msg)
        {
            std::cout << clientID << std::endl;
            msg.getData();
            // Action simple
        };
        EXPECT_NO_THROW(server->defineAction(i, action));
    }
}

// Tests pour les messages avec différents types de données
class MessageComplexTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        message = std::make_unique<Message>(10);
    }

    std::unique_ptr<Message> message;
};

TEST_F(MessageComplexTest, MessageWithMultipleDataTypes)
{
    int         testInt    = 42;
    double      testDouble = 3.14159;
    std::string testString = "Test String";

    *message << testInt << testDouble << testString;

    int         retrievedInt;
    double      retrievedDouble;
    std::string retrievedString;

    *message >> retrievedString >> retrievedDouble >> retrievedInt;

    EXPECT_EQ(retrievedInt, testInt);
    EXPECT_DOUBLE_EQ(retrievedDouble, testDouble);
    EXPECT_EQ(retrievedString, testString);
}

TEST_F(MessageComplexTest, MessageLargeString)
{
    std::string largeString(1000, 'A');
    *message << largeString;

    std::cout << "coucou" << std::endl;
    std::string retrievedString;
    *message >> retrievedString;

    EXPECT_EQ(retrievedString, largeString);
}

// Tests pour la gestion des erreurs
class ErrorHandlingTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        client = std::make_unique<Client>();
    }

    void TearDown() override
    {
        if (client)
        {
            client->disconnect();
        }
    }

    std::unique_ptr<Client> client;
};

TEST_F(ErrorHandlingTest, ClientThrowsOnInvalidIP)
{
    EXPECT_THROW(client->connect("999.999.999.999", 8080), std::runtime_error);
}

TEST_F(ErrorHandlingTest, ClientThrowsOnInvalidAddress)
{
    EXPECT_THROW(client->connect("not.a.valid.address", 8080), std::runtime_error);
}

// Tests pour les méthodes de Message avec données vides
class MessageEdgeCaseTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        message = std::make_unique<Message>(20);
    }

    std::unique_ptr<Message> message;
};

TEST_F(MessageEdgeCaseTest, MessageWithEmptyString)
{
    std::string emptyString = "";
    *message << emptyString;

    std::string retrievedString;
    *message >> retrievedString;

    EXPECT_EQ(retrievedString, emptyString);
}

TEST_F(MessageEdgeCaseTest, MessageGetDataOnEmptyMessage)
{
    auto data = message->getData();
    // Un message vide devrait au moins contenir les headers
    EXPECT_GE(data.size(), sizeof(int) + sizeof(size_t));
}
