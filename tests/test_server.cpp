// #include <arpa/inet.h>
// // #include <gmock/gmock.h>
// #include <gtest/gtest.h>
// #include <netinet/in.h>
// #include <sys/socket.h>
// #include <unistd.h>

// #include <chrono>
// #include <thread>

// #include "../src/network/client/client.hpp"
// #include "../src/network/message/message.hpp"
// #include "../src/network/server/server.hpp"

// class ServerTest : public ::testing::Test
// {
// protected:
//     void SetUp() override
//     {
//         server   = std::make_unique<Server>();
//         client   = std::make_unique<Client>();
//         testPort = 8080;
//     }

//     void TearDown() override
//     {
//         if (server)
//         {
//             server->stop();
//         }
//         if (client)
//         {
//             client->disconnect();
//         }
//     }

//     std::unique_ptr<Server> server;
//     std::unique_ptr<Client> client;
//     size_t                  testPort;
// };

// // Tests pour le serveur avec des fonctionnalités de base
// TEST_F(ServerTest, ServerCreationAndDestruction)
// {
//     EXPECT_NO_THROW(Server testServer);
// }

// TEST_F(ServerTest, ServerDefineAction)
// {
//     bool          actionCalled = false;
//     Message::Type testType     = 1;

//     auto action = [&actionCalled](long long& clientID, const Message& msg)
//     {
//         std::cout << clientID << std::endl;
//         msg.getData();
//         actionCalled = true;
//     };

//     EXPECT_NO_THROW(server->defineAction(testType, action));
// }

// TEST_F(ServerTest, ServerStartWithInvalidPort)
// {
//     // Port 0 est généralement invalide pour les serveurs
//     EXPECT_THROW(server->start(80), std::runtime_error);
// }

// // Tests d'intégration Client-Server
// class ClientServerIntegrationTest : public ::testing::Test
// {
// protected:
//     void SetUp() override
//     {
//         server       = std::make_unique<Server>();
//         client       = std::make_unique<Client>();
//         testPort     = 8888;
//         serverThread = nullptr;
//     }

//     void TearDown() override
//     {
//         if (serverThread && serverThread->joinable())
//         {
//             server->stop();
//             serverThread->join();
//         }
//         if (client)
//         {
//             client->disconnect();
//         }
//     }

//     void StartServerInThread()
//     {
//         serverThread = std::make_unique<std::thread>(
//             [this]()
//             {
//                 try
//                 {
//                     server->start(testPort);
//                 }
//                 catch (const std::exception& e)
//                 {
//                     // Server stopped normally
//                     std::cerr << "Server failed to start: " << e.what() << std::endl;
//                 }
//             });

//         // Petite pause pour laisser le serveur démarrer
//         std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     }

//     std::unique_ptr<Server>      server;
//     std::unique_ptr<Client>      client;
//     std::unique_ptr<std::thread> serverThread;
//     size_t                       testPort;
// };

// TEST_F(ClientServerIntegrationTest, ServerStartsSuccessfully)
// {
//     EXPECT_NO_THROW(StartServerInThread());
// }

// TEST_F(ClientServerIntegrationTest, ClientCanConnectToServer)
// {
//     StartServerInThread();

//     // Tentative de connexion du client
//     EXPECT_NO_THROW(client->connect("127.0.0.1", testPort));
// }

// // Tests de stress et edge cases
// class ServerStressTest : public ::testing::Test
// {
// protected:
//     void SetUp() override
//     {
//         server   = std::make_unique<Server>();
//         testPort = 8083;
//     }

//     void TearDown() override
//     {
//         if (server)
//         {
//             server->stop();
//         }
//     }

//     std::unique_ptr<Server> server;
//     size_t                  testPort;
// };

// TEST_F(ServerStressTest, ServerHandlesMultipleActionDefinitions)
// {
//     for (int i = 0; i < 100; ++i)
//     {
//         auto action = [i](long long& clientID, const Message& msg)
//         {
//             std::cout << clientID << std::endl;
//             msg.getData();
//             (void)i;
//         };
//         EXPECT_NO_THROW(server->defineAction(i, action));
//     }
// }

// // Tests pour la gestion des messages
// class ServerMessageTest : public ::testing::Test
// {
// protected:
//     void SetUp() override
//     {
//         server       = std::make_unique<Server>();
//         testPort     = 8084;
//         serverThread = nullptr;
//     }

//     void TearDown() override
//     {
//         if (serverThread && serverThread->joinable())
//         {
//             server->stop();
//             serverThread->join();
//         }
//     }

//     void StartServerInThread()
//     {
//         serverThread = std::make_unique<std::thread>(
//             [this]()
//             {
//                 try
//                 {
//                     server->start(testPort);
//                 }
//                 catch (const std::exception& e)
//                 {
//                     // Server stopped normally
//                 }
//             });
//         std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//     }

//     int CreateClientSocket()
//     {
//         int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
//         if (clientSocket < 0)
//             return -1;

//         sockaddr_in serverAddr;
//         serverAddr.sin_family      = AF_INET;
//         serverAddr.sin_port        = htons(testPort);
//         serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

//         if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
//         {
//             close(clientSocket);
//             return -1;
//         }
//         return clientSocket;
//     }

//     std::unique_ptr<Server>      server;
//     std::unique_ptr<std::thread> serverThread;
//     size_t                       testPort;
// };

// TEST_F(ServerMessageTest, ServerSendToSingleClient)
// {
//     StartServerInThread();

//     // Créer un message de test
//     Message testMessage(42);
//     testMessage << std::string("Hello World");

//     // Le test vérifie juste que sendTo ne lance pas d'exception
//     // avec un clientID qui n'existe pas (comportement défensif)
//     EXPECT_NO_THROW(server->sendTo(testMessage, 1));
// }

// TEST_F(ServerMessageTest, ServerSendToArray)
// {
//     StartServerInThread();

//     Message testMessage(42);
//     testMessage << std::string("Broadcast test");

//     std::vector<long long> clientIDs = {1, 2, 3};

//     // Test avec des clients inexistants
//     EXPECT_NO_THROW(server->sendToArray(testMessage, clientIDs));
// }

// TEST_F(ServerMessageTest, ServerSendToAll)
// {
//     StartServerInThread();

//     Message testMessage(42);
//     testMessage << std::string("Global broadcast");

//     // Test sendToAll sans clients connectés
//     EXPECT_NO_THROW(server->sendToAll(testMessage));
// }

// // Tests de gestion des connexions multiples
// class ServerMultiClientTest : public ::testing::Test
// {
// protected:
//     void SetUp() override
//     {
//         server       = std::make_unique<Server>();
//         testPort     = 8085;
//         serverThread = nullptr;
//     }

//     void TearDown() override
//     {
//         // Fermer tous les sockets clients
//         for (int socket : clientSockets)
//         {
//             if (socket >= 0)
//                 close(socket);
//         }
//         clientSockets.clear();

//         if (serverThread && serverThread->joinable())
//         {
//             server->stop();
//             serverThread->join();
//         }
//     }

//     void StartServerInThread()
//     {
//         serverThread = std::make_unique<std::thread>(
//             [this]()
//             {
//                 try
//                 {
//                     server->start(testPort);
//                 }
//                 catch (const std::exception& e)
//                 {
//                     // Server stopped normally
//                 }
//             });
//         std::this_thread::sleep_for(std::chrono::milliseconds(100));
//     }

//     int CreateAndConnectClient()
//     {
//         int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
//         if (clientSocket < 0)
//             return -1;

//         sockaddr_in serverAddr;
//         serverAddr.sin_family      = AF_INET;
//         serverAddr.sin_port        = htons(testPort);
//         serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

//         if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
//         {
//             close(clientSocket);
//             return -1;
//         }

//         clientSockets.push_back(clientSocket);
//         return clientSocket;
//     }

//     std::unique_ptr<Server>      server;
//     std::unique_ptr<std::thread> serverThread;
//     std::vector<int>             clientSockets;
//     size_t                       testPort;
// };

// TEST_F(ServerMultiClientTest, ServerHandlesMultipleConnections)
// {
//     StartServerInThread();

//     // Tenter de connecter plusieurs clients
//     for (int i = 0; i < 5; ++i)
//     {
//         int clientSocket = CreateAndConnectClient();
//         EXPECT_GE(clientSocket, 0) << "Failed to connect client " << i;

//         // Petite pause entre les connexions
//         std::this_thread::sleep_for(std::chrono::milliseconds(10));
//     }

//     EXPECT_EQ(clientSockets.size(), 5);
// }

// TEST_F(ServerMultiClientTest, ServerHandlesClientDisconnection)
// {
//     StartServerInThread();

//     // Connecter quelques clients
//     for (int i = 0; i < 3; ++i)
//     {
//         CreateAndConnectClient();
//     }

//     // Fermer le premier client
//     if (!clientSockets.empty())
//     {
//         close(clientSockets[0]);
//         clientSockets[0] = -1;
//     }

//     // Le serveur devrait continuer à fonctionner
//     std::this_thread::sleep_for(std::chrono::milliseconds(50));

//     // Connecter un nouveau client pour vérifier que le serveur fonctionne toujours
//     int newClient = CreateAndConnectClient();
//     EXPECT_GE(newClient, 0);
// }

// // Tests de robustesse et cas d'erreur
// class ServerRobustnessTest : public ::testing::Test
// {
// protected:
//     void SetUp() override
//     {
//         server   = std::make_unique<Server>();
//         testPort = 8086;
//     }

//     void TearDown() override
//     {
//         if (server)
//         {
//             server->stop();
//         }
//     }

//     std::unique_ptr<Server> server;
//     size_t                  testPort;
// };

// TEST_F(ServerRobustnessTest, ServerStopWithoutStart)
// {
//     // Arrêter un serveur qui n'a jamais été démarré
//     EXPECT_NO_THROW(server->stop());
// }

// TEST_F(ServerRobustnessTest, ServerMultipleStops)
// {
//     // Arrêter plusieurs fois de suite
//     EXPECT_NO_THROW(server->stop());
//     EXPECT_NO_THROW(server->stop());
//     EXPECT_NO_THROW(server->stop());
// }

// TEST_F(ServerRobustnessTest, ServerStartOnUsedPort)
// {
//     // Créer un premier serveur
//     auto server1 = std::make_unique<Server>();

//     std::thread serverThread1(
//         [&]()
//         {
//             try
//             {
//                 server1->start(testPort);
//             }
//             catch (const std::exception& e)
//             {
//                 // Expected
//             }
//         });

//     std::this_thread::sleep_for(std::chrono::milliseconds(100));

//     // Essayer de démarrer un second serveur sur le même port
//     EXPECT_THROW(server->start(testPort), std::runtime_error);

//     server1->stop();
//     if (serverThread1.joinable())
//         serverThread1.join();
// }

// TEST_F(ServerRobustnessTest, ServerSendEmptyMessage)
// {
//     Message emptyMessage(0);

//     // Envoyer un message vide ne devrait pas causer de crash
//     EXPECT_NO_THROW(server->sendTo(emptyMessage, 1));
//     EXPECT_NO_THROW(server->sendToAll(emptyMessage));
// }

// // Tests de performance et stress
// class ServerPerformanceTest : public ::testing::Test
// {
// protected:
//     void SetUp() override
//     {
//         server       = std::make_unique<Server>();
//         testPort     = 8087;
//         serverThread = nullptr;
//     }

//     void TearDown() override
//     {
//         for (int socket : clientSockets)
//         {
//             if (socket >= 0)
//                 close(socket);
//         }
//         clientSockets.clear();

//         if (serverThread && serverThread->joinable())
//         {
//             server->stop();
//             serverThread->join();
//         }
//     }

//     void StartServerInThread()
//     {
//         serverThread = std::make_unique<std::thread>(
//             [this]()
//             {
//                 try
//                 {
//                     server->start(testPort);
//                 }
//                 catch (const std::exception& e)
//                 {
//                     // Server stopped normally
//                 }
//             });
//         std::this_thread::sleep_for(std::chrono::milliseconds(100));
//     }

//     std::unique_ptr<Server>      server;
//     std::unique_ptr<std::thread> serverThread;
//     std::vector<int>             clientSockets;
//     size_t                       testPort;
// };

// TEST_F(ServerPerformanceTest, ServerHandlesRapidActionDefinitions)
// {
//     auto start = std::chrono::high_resolution_clock::now();

//     // Définir 1000 actions rapidement
//     for (int i = 0; i < 1000; ++i)
//     {
//         auto action = [](long long& clientID, const Message& msg)
//         {
//             (void)clientID;
//             (void)msg;
//         };
//         server->defineAction(i, action);
//     }

//     auto end      = std::chrono::high_resolution_clock::now();
//     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

//     // Devrait prendre moins d'une seconde
//     EXPECT_LT(duration.count(), 1000);
// }

// TEST_F(ServerPerformanceTest, ServerHandlesLargeMessages)
// {
//     StartServerInThread();

//     // Créer un gros message
//     Message     largeMessage(999);
//     std::string largeData(10000, 'A'); // 10KB de données
//     largeMessage << largeData;

//     // Devrait pouvoir gérer de gros messages sans crash
//     EXPECT_NO_THROW(server->sendToAll(largeMessage));
// }

// // Tests d'intégration avancés
// class ServerAdvancedIntegrationTest : public ::testing::Test
// {
// protected:
//     void SetUp() override
//     {
//         server           = std::make_unique<Server>();
//         testPort         = 8088;
//         serverThread     = nullptr;
//         actionCalled     = false;
//         receivedClientID = -1;
//     }

//     void TearDown() override
//     {
//         for (int socket : clientSockets)
//         {
//             if (socket >= 0)
//                 close(socket);
//         }
//         clientSockets.clear();

//         if (serverThread && serverThread->joinable())
//         {
//             server->stop();
//             serverThread->join();
//         }
//     }

//     void StartServerInThread()
//     {
//         serverThread = std::make_unique<std::thread>(
//             [this]()
//             {
//                 try
//                 {
//                     server->start(testPort);
//                 }
//                 catch (const std::exception& e)
//                 {
//                     // Server stopped normally
//                 }
//             });
//         std::this_thread::sleep_for(std::chrono::milliseconds(100));
//     }

//     std::unique_ptr<Server>      server;
//     std::unique_ptr<std::thread> serverThread;
//     std::vector<int>             clientSockets;
//     size_t                       testPort;
//     bool                         actionCalled;
//     long long                    receivedClientID;
// };

// TEST_F(ServerAdvancedIntegrationTest, ServerActionTriggering)
// {
//     // Définir une action qui sera appelée
//     Message::Type testType = 42;
//     server->defineAction(testType,
//                          [this](long long& clientID, const Message& msg)
//                          {
//                              actionCalled     = true;
//                              receivedClientID = clientID;
//                              (void)msg;
//                          });

//     StartServerInThread();

//     // Note: Pour un test complet, il faudrait envoyer un message depuis un client
//     // et vérifier que l'action est appelée, mais cela nécessiterait une implémentation
//     // complète du client ou l'envoi manuel de données via socket
// }

// TEST_F(ServerAdvancedIntegrationTest, ServerActionOverride)
// {
//     Message::Type testType           = 42;
//     bool          firstActionCalled  = false;
//     bool          secondActionCalled = false;

//     // Première action
//     server->defineAction(testType,
//                          [&firstActionCalled](long long& clientID, const Message& msg)
//                          {
//                              firstActionCalled = true;
//                              (void)clientID;
//                              (void)msg;
//                          });

//     // Redéfinir l'action (devrait remplacer la première)
//     server->defineAction(testType,
//                          [&secondActionCalled](long long& clientID, const Message& msg)
//                          {
//                              secondActionCalled = true;
//                              (void)clientID;
//                              (void)msg;
//                          });

//     // Les deux définitions devraient réussir
//     EXPECT_TRUE(true); // Le fait qu'on arrive ici signifie que les defineAction ont réussi
// }