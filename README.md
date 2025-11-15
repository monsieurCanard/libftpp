<div align="center">
  <img width="80%" alt="Libftpp Banner" src="https://github.com/user-attachments/assets/97c2fe5a-16d8-4526-8447-533ed9501395" />
</div>

<h1 align="center">Libftpp 📚</h1>

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-17-00599C?logo=cplusplus&logoColor=white" alt="C++17"/>
  <a href="https://libftpp.dukiverse.com"><img src="https://img.shields.io/badge/docs-doxygen-blue.svg" alt="Documentation"/></a>
  <img src="https://img.shields.io/badge/tests-GoogleTest-red?logo=google&logoColor=white" alt="GoogleTest"/>

</p>

# 👀 Vue d'ensemble

## 📖 Description

**libftpp** est une librairie C++ moderne explorant les structures de données avancées et les design patterns. Ce projet met l'accent sur les bonnes pratiques du C++ moderne, la gestion efficace de la mémoire et l'implémentation rigoureuse des patterns classiques du génie logiciel.

## 📚 Documentation

Une **documentation complète générée avec Doxygen** est disponible en ligne :

🔗 **[libftpp.dukiverse.com](https://libftpp.dukiverse.com)**

Cette documentation inclut :
- 📖 API complète de toutes les classes et fonctions
- 💡 Exemples d'utilisation détaillés
- 🔍 Diagrammes de classes et relations
- 📝 Guide d'implémentation des design patterns
- ⚡ Notes sur les performances et bonnes pratiques

## 🚀 Installation et compilation

### Prérequis
- Compilateur C++17 ou supérieur (g++ recommandé)
- CMake 3.14 ou supérieur (pour les tests GoogleTest)
- Make
- Git

### Installation

```bash
git clone git@github.com:monsieurCanard/libftpp.git
cd libftpp
```

### Compilation de la librairie

```bash
make
```
Cette commande compile tous les fichiers sources et génère la librairie statique `libftpp.a` dans le répertoire racine.

**Fichiers compilés :**
- Structures de données (DataBuffer)
- Design Patterns (Memento)
- Réseau (Message, Server, Client)
- Mathématiques (Perlin Noise, Random 2D Coordinate Generator)
- Threading (ThreadSafeIOStream, PersistentWorker, Thread, WorkerPool)
- Bonus (Logger, Chronometre, RingBuffer)

**Options de compilation :**
- `-std=c++17` : Standard C++17
- `-Wall -Wextra -Werror` : Tous les warnings activés, traités comme des erreurs

### Compilation et lancement des tests Google Test

```bash
# Compiler la librairie + les tests GoogleTest
make gtest

# Lancer les tests GoogleTest
make run-gtest
```

Les tests GoogleTest sont automatiquement téléchargés via CMake et compilés dans le dossier `build/`.

**Tests disponibles :**
- `test_data_buffer.cpp` - Tests du buffer de données
- `test_pool.cpp` - Tests du pool mémoire
- `test_memento.cpp` - Tests du pattern Memento
- `test_observer.cpp` - Tests du pattern Observer
- `test_singleton.cpp` - Tests du pattern Singleton
- `test_state_machine.cpp` - Tests de la machine à états
- `test_message.cpp` - Tests du système de messages
- `test_thread.cpp` - Tests des threads
- `test_thread_safe_queue.cpp` - Tests de la queue thread-safe
- `test_worker_pool.cpp` - Tests du pool de workers
- `test_persistent_worker.cpp` - Tests du worker persistant
- `test_logger.cpp` - Tests du logger
- `test_chronometre.cpp` - Tests du chronomètre
- `test_ring_buffer.cpp` - Tests du buffer circulaire
- `test_n_ary_tree.cpp` - Tests de l'arbre n-aire
- `test_observable_value.cpp` - Tests de la valeur observable
- `test_ivector2.cpp` - Tests du vecteur 2D
- `test_ivector3.cpp` - Tests du vecteur 3D
- `test_perlin_noise.cpp` - Tests du bruit de Perlin
- `test_random_2D_coordinate_generator.cpp` - Tests du générateur de coordonnées

### Nettoyage

```bash
# Nettoyer les fichiers objets
make clean

# Nettoyer complètement (objets + librairie + tests + logs)
make fclean

# Recompiler complètement
make re
```


## 📁 Structure du projet

```
libftpp/
├── src/
│   ├── data_structures/
│   │   ├── data_buffer/         # Sérialisation/désérialisation de données
│   │   └── pool/                # Pool de mémoire avec allocation optimisée
│   ├── design_patterns/
│   │   ├── memento/             # Sauvegarde et restauration d'état
│   │   ├── observer/            # Notification d'événements
│   │   ├── singleton/           # Instance unique globale
│   │   └── state_machine/       # Machine à états finis
│   ├── mathematics/
│   │   ├── IVector2/            # Interface vecteur 2D
│   │   ├── IVector3/            # Interface vecteur 3D
│   │   ├── perlin_noise/        # Génération de bruit de Perlin
│   │   └── random_2D_coordinate_generator/  # Générateur de coordonnées aléatoires
│   ├── network/
│   │   ├── client/              # Client TCP pour communication réseau
│   │   ├── message/             # Système de messages structurés
│   │   └── server/              # Serveur TCP multi-clients avec select()
│   ├── thread/
│   │   ├── persistent_worker/   # Worker thread persistant
│   │   ├── thread/              # Wrapper thread avec fonctionnalités étendues
│   │   ├── thread_safe_iostream/ # IO thread-safe
│   │   ├── thread_safe_queue/   # Queue thread-safe
│   │   └── worker_pool/         # Pool de threads workers
│   └── bonus/
│       ├── chronometre/         # Mesure de temps et performance
│       ├── logger/              # Système de logging avancé
│       ├── n_ary_tree/          # Arbre n-aire générique
│       ├── observable_value/    # Valeur observable avec pattern Observer
│       │   └── IObserver/       # Interface observateur
│       └── ring_buffer/         # Buffer circulaire optimisé
├── tests/                       # Tests unitaires avec GoogleTest
│   ├── test_*.cpp              # Tests pour chaque composant
│   └── main.cpp                # Point d'entrée des tests
├── programs_test/               # Programmes d'exemple et de test
│   ├── programServer/          # Exemple serveur de test
│   └── programStressTesteur/   # Programme de test de charge
├── CMakeLists.txt              # Configuration CMake
├── Makefile                    # Build système Make
└── libftpp.hpp                 # Header unifié pour toute la librairie
```

## 🎯 Règles de codage

- **Classes** : PascalCase (`MyClass`)
- **Méthodes** : camelCase (`myMethod()`)
- **Fichiers** : snake_case (`my_class.hpp`)
- **Standard** : C++11 minimum avec flags `-Wall -Wextra -Werror`
- **Interdictions** : Boost, `*printf()`, `*alloc()`, `free()`
- **Headers** : Indépendants avec include guards
- **Mémoire** : Éviter les fuites, gestion RAII (Resource Acquisition Is Initialization)

# 📖 Classes implémentées

---
## 📚 Structures de données implémentées

### 📦 Pool de mémoire

Un **Pool** est un réservoir de mémoire qui pré-alloue des objets pour éviter les allocations/désallocations fréquentes. Améliore significativement les performances pour la création d'objets coûteux.

**Caractéristiques :**
- Utilise `std::aligned_storage` pour l'alignement mémoire (deprecated C++23, utiliser `std::aligned_storage_t`)
- Templates variadiques pour les constructeurs
- Placement new pour la construction in-place
- Gestion automatique de la mémoire via RAII

**API principale :**
```cpp
Pool<MyClass> pool(100);  // Pré-alloue 100 objets
auto* obj = pool.acquire(arg1, arg2);  // Construction avec arguments
pool.release(obj);  // Libération pour réutilisation
```

**Avantages :**
- Évite la fragmentation mémoire
- Réduction drastique des appels `new`/`delete`
- Performance prévisible pour les systèmes temps réel

### 💾 DataBuffer

Système de **sérialisation/désérialisation** thread-safe utilisant un buffer interne pour la persistance et le transfert de données.

**Fonctionnalités :**
- Sérialisation via `reinterpret_cast<unsigned int*>` pour copier les octets bruts
- Désérialisation via `memcpy` pour restaurer les données typées
- Support des types primitifs et `std::string`
- Gestion automatique de la taille et du curseur
- Operators `<<` et `>>` pour une syntaxe intuitive

**Utilisation :**
```cpp
DataBuffer buffer;

// Sérialisation
buffer << 42 << 3.14f << std::string("Hello");

// Désérialisation (ordre important!)
int value;
float pi;
std::string text;
buffer >> value >> pi >> text;

// Accès bas niveau
buffer.appendBytes(data, size);
auto raw = buffer.getBytes();
```

**Cas d'usage :**
- Communication réseau (utilisé par la classe `Message`)
- Sauvegarde de jeu
- IPC (Inter-Process Communication)
- Cache de données

---
## 🧵 Programmation concurrente et Threading

La librairie fournit plusieurs composants pour la programmation multi-thread sécurisée et performante.

### 🧵 Thread

Wrapper autour de `std::thread` avec des fonctionnalités étendues pour faciliter la gestion des threads.

**Caractéristiques :**
- Encapsulation de `std::thread` avec interface simplifiée
- Gestion automatique du cycle de vie
- Support des lambdas et fonctions membres

**Utilisation :**
```cpp
Thread worker([]() {
    // Code exécuté dans le thread
    processData();
});

worker.join();  // Attendre la fin
```

### 🔐 ThreadSafeQueue

Queue FIFO thread-safe utilisant mutex et condition variables pour la synchronisation.

**Fonctionnalités :**
- Operations `push()` et `pop()` thread-safe
- Blocage automatique si la queue est vide
- Gestion propre de l'arrêt

**Pattern producteur-consommateur :**
```cpp
ThreadSafeQueue<Task> queue;

// Thread producteur
queue.push(task);

// Thread consommateur
Task task = queue.pop();  // Bloque si vide
```

### 🖨️ ThreadSafeIOStream

Wrapper thread-safe pour les opérations I/O (cout, cerr, fichiers).

**Problème résolu :**
Les streams C++ standards ne sont pas thread-safe, ce qui peut causer des sorties entrelacées.

**Solution :**
```cpp
ThreadSafeIOStream tsout(std::cout);

// Dans différents threads
tsout << "Thread 1: " << data << std::endl;
tsout << "Thread 2: " << other << std::endl;
// Garantit que chaque ligne est atomique
```

### ⚙️ PersistentWorker

Thread worker qui exécute des tâches nommées en boucle continue avec gestion dynamique.

**Architecture :**
- Map de tâches `unordered_map<string, function<void()>>`
- Ajout/suppression dynamique pendant l'exécution
- Pause configurable entre tâches (`PAUSE_BT_TASK`)

**Utilisation :**
```cpp
PersistentWorker worker;

// Ajouter des tâches qui s'exécutent en boucle
worker.addTask("heartbeat", []() {
    sendHeartbeat();
});

worker.addTask("monitor", []() {
    checkSystemHealth();
});

// Supprimer une tâche
worker.removeTask("heartbeat");

// Arrêt automatique à la destruction
```

**Cas d'usage :**
- Monitoring système
- Heartbeats réseau
- Nettoyage périodique
- Mise à jour de caches

### 🔄 (astuce) Condition Variables

Les **condition variables** (`std::condition_variable`) permettent la synchronisation entre threads en bloquant un thread jusqu'à ce qu'une condition soit remplie.

**Principe :**
Une condition variable est toujours associée à un mutex et permet à un thread d'attendre qu'un autre thread signale un changement d'état.

**Utilisation typique :**
```cpp
std::mutex mtx;
std::condition_variable cv;
bool ready = false;

// Thread producteur
{
    std::lock_guard<std::mutex> lock(mtx);
    ready = true;
}
cv.notify_one(); // Réveille un thread en attente

// Thread consommateur  
{
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [] { return ready; }); // Attend que ready soit true
}
```

**Méthodes principales :**
- `wait(lock, predicate)` : Bloque jusqu'à ce que le prédicat soit vrai
- `notify_one()` : Réveille un thread en attente
- `notify_all()` : Réveille tous les threads en attente

**Pattern Worker Pool :**
```cpp
// Condition pour réveiller les workers
cv.wait(lock, [this] { return stop || !jobs.empty(); });
// Se réveille quand il y a du travail OU qu'on demande l'arrêt
```

### 🏭 WorkerPool

Un **WorkerPool** est un pattern de concurrence qui maintient un groupe de threads workers qui exécutent des tâches à partir d'une queue partagée.

**Architecture :**
- **Queue de jobs** : `std::queue<std::function<void()>>`
- **Workers** : `std::vector<std::thread>` qui exécutent la boucle principale
- **Synchronisation** : `std::mutex` + `std::condition_variable`

**Fonctionnement :**
```cpp
WorkerPool pool(4); // 4 threads workers
pool.addJob([]() { 
    // Votre tâche ici
    processData(); 
});
// Le destructeur attend que tous les jobs se terminent
```

**Avantages :**
- Évite la création/destruction répétée de threads
- Contrôle du niveau de parallélisme
- Distribution automatique des tâches
- Gestion propre de l'arrêt

**Pattern de boucle worker :**
```cpp
void WorkerPool::loop() {
    while (!_stop) {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(_mtx);
            _cv.wait(lock, [this] { return _stop || !_jobs.empty(); });
            
            if (_stop || _jobs.empty()) break;
            
            job = _jobs.front();
            _jobs.pop();
        }
        job(); // Exécution hors du lock
    }
}
```
---
## 🌐 Architecture réseau

### 📨 Message

Classe pour la communication réseau structurée utilisant un protocole de messaging personnalisé.

**Format de transfert :**
```
[Type (int)][Taille (size_t)][Données (variable)]
```

**Caractéristiques :**
- **Sérialisation automatique** : Operators `<<` et `>>` pour tous types
- **RingBuffer interne** : Stockage efficace des données  
- **Type safety** : Chaque message a un type pour le dispatch
- **Gestion des strings** : Spécialisations pour `std::string`

**Utilisation :**
```cpp
Message msg(MESSAGE_LOGIN);
msg << userId << username << password;

// Côté réception
int userId;
string username, password;  
msg >> userId >> username >> password;
```

**API principale :**
- `operator<<(const T&)` : Ajout de données typées
- `operator>>(T&)` : Extraction de données typées  
- `getSerializedData()` : Données complètes pour transmission
- `isComplet()` : Vérification de l'intégrité du message

### 🖥️ Server

Serveur TCP multi-clients utilisant `select()` pour la gestion asynchrone des connexions.

**Architecture :**
- **Sockets POSIX** : API système pour les communications TCP
- **select()** : Multiplexage I/O pour gérer plusieurs clients
- **Callbacks** : Actions définissables par type de message
- **Gestion d'état** : Tracking des connexions et messages partiels

**Fonctionnalités :**
```cpp
Server server;
server.start(8080);

// Définir une action pour un type de message
server.defineAction(MSG_CHAT, [](long long clientId, const Message& msg) {
    std::string content;
    msg >> content;
    // Broadcast à tous les clients
    server.sendToAll(createChatMessage(content));
});

server.update(); // Traite les événements réseau
```

**Limitations :**
- **Non thread-safe** : Utilisation mono-thread uniquement
- **Connexions limitées** : Maximum `NB_CONNECTION` (256) clients
- **Buffer fixe** : `READ_BUFFER_SIZE` (4096) octets par lecture

### 💻 Client  

Client TCP utilisant les mêmes primitives que le serveur pour la communication.

**Fonctionnalités :**
- **Connexion simple** : `connect(address, port)`
- **Messaging** : Envoi/réception de messages structurés
- **Callbacks** : Actions sur réception de messages
- **Gestion d'état** : Reconstruction des messages fragmentés

**Utilisation :**
```cpp
Client client("127.0.0.1", 8080);

client.defineAction(MSG_WELCOME, [](const Message& msg) {
    std::string welcome;
    msg >> welcome;
    std::cout << welcome << std::endl;
});

Message loginMsg(MSG_LOGIN);
loginMsg << "username" << "password";
client.send(loginMsg);

client.update(); // Traite les messages entrants
```

**Pattern d'utilisation typique :**
```cpp
while (running) {
    client.update();           // Traite les messages entrants
    handleInputEvents();       // Interface utilisateur
    std::this_thread::sleep_for(16ms); // ~60 FPS
}
```
---
## 🔢 Mathématiques

La librairie fournit des outils mathématiques pour les graphiques, jeux et simulations.

### 📐 IVector2 et IVector3

Interfaces pour les vecteurs 2D et 3D avec opérations mathématiques standards.

**Opérations supportées :**
- Addition, soustraction, multiplication, division
- Produit scalaire (dot product)
- Norme et normalisation
- Distance entre vecteurs

**IVector2 - Vecteur 2D :**
```cpp
template <typename T>
class IVector2 {
    T x, y;
public:
    IVector2(T x = 0, T y = 0);
    
    IVector2 operator+(const IVector2& other) const;
    IVector2 operator-(const IVector2& other) const;
    IVector2 operator*(T scalar) const;
    
    T dot(const IVector2& other) const;
    T length() const;
    IVector2 normalized() const;
    T distance(const IVector2& other) const;
};
```

**IVector3 - Vecteur 3D :**
```cpp
template <typename T>
class IVector3 {
    T x, y, z;
public:
    IVector3(T x = 0, T y = 0, T z = 0);
    
    IVector3 operator+(const IVector3& other) const;
    IVector3 cross(const IVector3& other) const;  // Produit vectoriel
    T dot(const IVector3& other) const;
    T length() const;
    IVector3 normalized() const;
};
```

**Cas d'usage :**
- Physique et collisions
- Graphiques 2D/3D
- Calculs géométriques
- Déplacements et directions

### 🌊 Perlin Noise 2D

Générateur de bruit de Perlin pour créer des textures procédurales naturelles et cohérentes.

**Caractéristiques :**
- Génération de bruit pseudo-aléatoire mais cohérent
- Interpolation lisse entre valeurs
- Paramétrable (seed, fréquence, amplitude)
- Idéal pour terrains, textures, nuages

**Utilisation :**
```cpp
PerlinNoise2D perlin(seed);

// Générer une heightmap pour un terrain
for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
        float noise = perlin.noise(x * 0.1f, y * 0.1f);
        // noise est entre -1.0 et 1.0
        heightmap[y][x] = (noise + 1.0f) * 0.5f;  // Normaliser à [0, 1]
    }
}
```

**Applications :**
- Génération de terrains procéduraux
- Textures naturelles (bois, marbre, nuages)
- Animation de particules
- Génération de cartes

### 🎲 Random 2D Coordinate Generator

Générateur de coordonnées aléatoires 2D avec distribution uniforme.

**Fonctionnalités :**
- Génération de points aléatoires dans un rectangle
- Seed configurable pour reproductibilité
- Distribution uniforme garantie

**Utilisation :**
```cpp
Random2DCoordinateGenerator gen(seed);

// Générer des positions de spawning
for (int i = 0; i < enemyCount; i++) {
    auto [x, y] = gen.generate(0, mapWidth, 0, mapHeight);
    spawnEnemy(x, y);
}
```

**Cas d'usage :**
- Placement d'objets dans un jeu
- Génération de niveaux procéduraux
- Tests avec données aléatoires
- Simulations Monte Carlo
---
## 🎁 Composants Bonus


### ⏱️ Chronometre

Classe utilitaire pour mesurer précisément le temps d'exécution et les performances.

**Fonctionnalités :**
- Mesure de temps haute précision avec `std::chrono`
- Calcul automatique du temps écoulé
- Support de plusieurs unités (ms, µs, ns)

**Utilisation :**
```cpp
Chronometre chrono;
chrono.start();

// Code à mesurer
expensiveOperation();

chrono.stop();
std::cout << "Temps: " << chrono.elapsed() << " ms" << std::endl;
```

### 📝 Logger

Système de logging thread-safe avec niveaux de log et sortie fichier/console.

**Niveaux de log :**
- `DEBUG` : Messages de débogage détaillés
- `INFO` : Messages informatifs généraux
- `WARNING` : Avertissements
- `ERROR` : Erreurs critiques

**Caractéristiques :**
- Singleton pattern pour accès global
- Thread-safe via `ThreadSafeIOStream`
- Horodatage automatique
- Filtrage par niveau de log
- Sortie fichier et/ou console

**Utilisation :**
```cpp
// Configuration
Logger::instance().setOutputFile("app.log");
Logger::instance().setLogLevel(LogLevel::INFO);

// Logging
Logger::instance().log(LogLevel::INFO, "Application started");
Logger::instance().log(LogLevel::ERROR, "Connection failed");

// Console uniquement
Logger::instance().logConsole(LogLevel::DEBUG, "Debug info");
```

### 🔄 RingBuffer

Buffer circulaire optimisé avec taille fixe et écrasement automatique des anciennes données.

**Avantages :**
- Pas de réallocation dynamique
- Performance prévisible O(1)
- Idéal pour les logs, audio, streaming

**Utilisation :**
```cpp
RingBuffer<int> buffer(1000);  // Capacité de 1000 éléments

buffer.push(42);
int value = buffer.pop();

if (buffer.isFull()) {
    // Les nouvelles données écrasent les anciennes
}
```

**Cas d'usage :**
- Buffers audio/vidéo
- Logs circulaires
- File d'événements
- Données de capteurs

### 🌳 N-ary Tree

Arbre générique où chaque nœud peut avoir N enfants (pas limité à 2 comme un arbre binaire).

**Caractéristiques :**
- Template pour tout type de données
- Parcours en profondeur et largeur
- Insertion/suppression dynamique
- Gestion automatique de la mémoire

**Utilisation :**
```cpp
NAryTree<std::string> tree;
auto* root = tree.createRoot("Root");
auto* child1 = tree.addChild(root, "Child 1");
auto* child2 = tree.addChild(root, "Child 2");
tree.addChild(child1, "Grand Child");

// Parcours
tree.traverseDepthFirst([](const std::string& data) {
    std::cout << data << std::endl;
});
```

**Cas d'usage :**
- Systèmes de fichiers
- Organigrammes
- Hiérarchies de scènes 3D
- Arbres de décision

### 👁️ ObservableValue

Template qui implémente le pattern Observer pour des valeurs observables avec notification automatique.

**Fonctionnalités :**
- Notification automatique lors du changement de valeur
- Support de multiples observateurs
- Comparaison pour éviter les notifications inutiles
- Gestion automatique des abonnements

**Interface requise :**
```cpp
template <typename T>
class IObserver {
public:
    virtual void update(const T& newValue) = 0;
};
```

**Utilisation :**
```cpp
class MyObserver : public IObserver<int> {
public:
    void update(const int& value) override {
        std::cout << "Nouvelle valeur: " << value << std::endl;
    }
};

ObservableValue<int> health(100);
MyObserver observer;

health.subscribe(&observer);
health.set(95);  // Déclenche observer.update(95)
health = 90;     // Via operator=, déclenche aussi
```

**Cas d'usage :**
- Reactive programming
- Data binding (UI)
- Systèmes d'événements
- Game state management



# 📚 Gang of Four – Résumé simple

| Catégorie | Pattern | Idée en une phrase | Exemple en C++ |
|-----------|---------|---------------------|----------------|
| **Création** | Singleton | Toujours la même instance unique | Un seul `Logger` ou `ConfigManager` |
| | Factory Method | Choisit quel objet créer sans dire son type exact | `ShapeFactory` → retourne `Circle` ou `Square` |
| | Abstract Factory | Crée des familles d’objets compatibles | `UIFactory` → boutons Windows/Linux |
| | Builder | Construit un objet étape par étape | `HttpRequestBuilder` pour configurer une requête |
| | Prototype | Clone un objet existant | `Document* copy = doc.clone();` |
| **Structure** | Adapter | Rend deux interfaces compatibles | `LegacyPrinterAdapter` pour utiliser une vieille lib |
| | Bridge | Sépare abstraction et implémentation | `Renderer` (OpenGL/DirectX) séparé de `Shape` |
| | Composite | Objets simples et composés traités pareil | `File` et `Directory` dans un système de fichiers |
| | Decorator | Ajoute des fonctions sans toucher au code | `Stream` décoré avec `BufferedStream` |
| | Facade | Simplifie un système complexe | `CompilerFacade` qui appelle lexer+parser+codegen |
| | Flyweight | Partage objets identiques pour économiser mémoire | `Character` dans un éditeur de texte |
| | Proxy | Contrôle l’accès à un objet réel | `ImageProxy` qui charge l’image à la demande |
| **Comportement** | Observer | Un objet prévient les autres automatiquement | `Button` → notifie ses listeners |
| | Memento | Sauvegarde/restaure un état | `Game.save()` et `Game.load()` |
| | State | Change le comportement selon l’état | `TCPConnection` en état `Connected`/`Closed` |
| | Chain of Responsibility | Passe une requête dans une chaîne de handlers | Middleware HTTP qui traite ou passe au suivant |
| | Command | Action emballée dans un objet | `UndoCommand` ou `MoveCommand` dans un éditeur |
| | Interpreter | Exécute une mini-grammaire/langage | Calculatrice qui lit `1+2*3` |
| | Iterator | Parcourt une collection sans connaître sa structure | `for(auto it = list.begin(); it != list.end(); ++it)` |
| | Mediator | Un objet central gère la communication | `ChatRoom` qui relaie les messages |
| | Strategy | Choisir un algo interchangeable facilement | `sort(data, QuickSortStrategy{})` |
| | Template Method | Squelette d’un algo, détails dans les sous-classes | `Game::play()` appelle `init()`, `loop()`, `end()` |
| | Visitor | Ajoute une opération sans changer les classes | `ASTVisitor` pour analyser un arbre syntaxique |


# 📂 Details - Design Patterns (✅ = implémentés)

## 🔹 Création (Creational Patterns)

### ✅ Singleton
**But :** Garantit qu'une classe n'a qu'une seule instance et fournit un point d'accès global.

**Utilisation :** Configuration globale, gestionnaire de ressources, logging.

**Implémentation :** Utilise `static` pour partager l'instance entre threads.

```cpp
Singleton<DatabaseManager> db;
db.instantiate("localhost", 5432);
auto* manager = db.instance();
```

**Avantages :**
- Contrôle strict de l'instanciation
- Accès global uniforme

### Factory Method
**But :** Définit une interface pour créer un objet, mais laisse les sous-classes décider de la classe instanciée.

**Utilisation :** Création d'objets sans spécifier leur classe exacte, frameworks extensibles.

**Exemple d'usage :**
```cpp
class ShapeFactory {
public:
    virtual Shape* createShape() = 0;
};

class CircleFactory : public ShapeFactory {
public:
    Shape* createShape() override { return new Circle(); }
};
```

### Abstract Factory
**But :** Fournit une interface pour créer des familles d'objets liés sans spécifier leurs classes concrètes.
**Utilisation :** Systèmes multi-plateformes, thèmes d'interface, familles de produits.

**Avantages :**
- Isole les classes concrètes
- Facilite l'échange de familles de produits
- Garantit la cohérence entre produits

### Builder
**But :** Sépare la construction complexe
 d'un objet de sa représentation finale.

**Utilisation :** Objets avec de nombreux paramètres optionnels, configurations complexes.

**Exemple :**
```cpp
class HttpRequestBuilder {
public:
    HttpRequestBuilder& setUrl(const std::string& url);
    HttpRequestBuilder& setMethod(const std::string& method);
    HttpRequestBuilder& addHeader(const std::string& key, const std::string& value);
    HttpRequest build();
};
```

### Prototype
**But :** Crée de nouveaux objets en copiant un prototype existant.

**Utilisation :** Clonage d'objets complexes, éviter la re-initialisation coûteuse.

## 🔹 Structure (Structural Patterns)

### Adapter
**But :** Convertit l'interface d'une classe en une autre attendue par le client.

**Utilisation :** Intégration de librairies tierces, legacy code, APIs incompatibles.

**Types :**
- **Object Adapter** : utilise la composition
- **Class Adapter** : utilise l'héritage multiple

### Bridge
**But :** Sépare l'abstraction de son implémentation pour les faire évoluer indépendamment.

**Utilisation :** Éviter l'explosion combinatoire d'héritages, architectures multi-plateformes.

**Avantages :**
- Découplage abstraction/implémentation
- Extensibilité facilitée
- Masquage des détails d'implémentation

### Composite
**But :** Permet de traiter un groupe d'objets comme un objet unique (structure d'arbre).

**Utilisation :** Systèmes de fichiers, interfaces graphiques, menus hiérarchiques.

**Caractéristiques :**
- Structure récursive
- Traitement uniforme des objets simples et composites
- Facilite l'ajout de nouveaux types de composants

### Decorator
**But :** Ajoute dynamiquement des responsabilités à un objet sans modifier sa classe.

**Utilisation :** Extensions de fonctionnalités, middleware, streams, GUI.

**Avantages :**
- Alternative flexible à l'héritage
- Combinaison dynamique de comportements
- Respect du principe Open/Closed

### Facade
**But :** Fournit une interface simplifiée à un ensemble complexe de classes/sous-systèmes.

**Utilisation :** APIs simplifiées, masquer la complexité interne, point d'entrée unifié.

**Bénéfices :**
- Réduction du couplage
- Interface plus simple
- Centralisation des interactions

### Flyweight
**But :** Partage les objets pour économiser mémoire/performances.

**Utilisation :** Nombreuses instances d'objets similaires (caractères, particules, tuiles).

**Concepts clés :**
- **État intrinsèque** : partagé entre instances
- **État extrinsèque** : unique à chaque contexte
- Factory pour gérer les instances partagées

### Proxy
**But :** Fournit un substitut qui contrôle l'accès à un objet réel.

**Utilisation :** Lazy loading, contrôle d'accès, cache, logging, réseaux.

**Types de proxy :**
- **Virtual Proxy** : création à la demande
- **Protection Proxy** : contrôle d'accès
- **Remote Proxy** : représentant local d'un objet distant

## 🔹 Comportement (Behavioral Patterns)

### ✅ Observer
**But :** Notifie automatiquement une liste d'observateurs lors d'un changement d'état.

**Utilisation :** Interfaces utilisateur, événements système, MVC, reactive programming.

**Implémentation dans libftpp :**
La librairie fournit deux implémentations du pattern Observer :
1. **Observer classique** : Interface générique pour implémenter vos propres observateurs
2. **ObservableValue** : Template spécialisé pour observer les changements de valeurs

**Avantages :**
- Découplage entre sujet et observateurs
- Communication broadcast
- Support dynamique d'observateurs
- Notification automatique uniquement en cas de changement réel

**Implémentation classique :**
```cpp
// Interface observer
template <typename T>
class IObserver {
public:
    virtual void update(const T& data) = 0;
    virtual ~IObserver() = default;
};

// Sujet observable
class Subject {
    std::vector<IObserver<int>*> observers;
public:
    void attach(IObserver<int>* obs) { observers.push_back(obs); }
    void detach(IObserver<int>* obs) { 
        observers.erase(std::remove(observers.begin(), observers.end(), obs));
    }
    void notify(int data) { 
        for(auto* obs : observers) obs->update(data); 
    }
};
```

**ObservableValue (bonus) :**
```cpp
ObservableValue<PlayerStats> stats;
StatsDisplay display;

stats.subscribe(&display);
stats.set(newStats);  // Notifie automatiquement display
```

### ✅ Memento
**But :** Sauvegarde/restaure l'état interne d'un objet sans violer l'encapsulation.

**Utilisation :** Undo/Redo, snapshots, sauvegarde d'état, checkpoints.

**Principe :** Externalise l'état sans exposer la structure interne.

**Structure :**
- **Originator** : crée et utilise les mementos
- **Memento** : stocke l'état interne
- **Caretaker** : gère les mementos sans les examiner

**Implémentation dans libftpp :**
```cpp
class Memento {
    friend class Originator;
private:
    std::string state;
    Memento(const std::string& s) : state(s) {}
public:
    // Pas d'accès direct à l'état
};

class Originator {
    std::string state;
public:
    Memento save() {
        return Memento(state);
    }
    
    void restore(const Memento& m) {
        state = m.state;
    }
    
    void setState(const std::string& s) { state = s; }
};

// Utilisation
Originator obj;
obj.setState("State1");
Memento backup = obj.save();

obj.setState("State2");
obj.restore(backup);  // Retour à State1
```

**Avantages :**
- Préserve l'encapsulation
- Simplifie l'implémentation d'undo/redo
- Sauvegarde sans exposer les détails internes

### ✅ State Machine
**But :** Change le comportement d'un objet selon son état interne.

**Utilisation :** Parsers, protocoles réseau, jeux (IA), workflows, automates.

**Avantages :**
- Code plus maintenable pour les logiques d'état complexes
- Élimination des conditions if/switch complexes
- États explicites et transitions claires
- Comportement polymorphique selon l'état

**Implémentation dans libftpp :**
```cpp
// Interface d'état
class State {
public:
    virtual void enter() = 0;
    virtual void execute() = 0;
    virtual void exit() = 0;
    virtual ~State() = default;
};

// États concrets
class IdleState : public State {
public:
    void enter() override { std::cout << "Entering Idle\n"; }
    void execute() override { /* Comportement idle */ }
    void exit() override { std::cout << "Exiting Idle\n"; }
};

class RunningState : public State {
public:
    void enter() override { std::cout << "Start running\n"; }
    void execute() override { /* Comportement running */ }
    void exit() override { std::cout << "Stop running\n"; }
};

// Machine à états
class StateMachine {
    State* currentState;
public:
    StateMachine(State* initial) : currentState(initial) {
        currentState->enter();
    }
    
    void transition(State* newState) {
        currentState->exit();
        currentState = newState;
        currentState->enter();
    }
    
    void update() {
        currentState->execute();
    }
};

// Usage
IdleState idle;
RunningState running;
StateMachine fsm(&idle);

fsm.update();           // Execute idle behavior
fsm.transition(&running); // Change to running
fsm.update();           // Execute running behavior
```

**Cas d'usage réels :**
- IA de jeu (Idle, Patrol, Attack, Flee)
- Connexions réseau (Disconnected, Connecting, Connected)
- UI (Loading, Menu, Playing, Paused)
- Protocoles (Handshake, Data Transfer, Closing)

### Chain of Responsibility
**But :** Passe une requête à travers une chaîne d'objets susceptibles de la traiter.

**Utilisation :** Validation, middleware, gestionnaires d'événements, parsers.

**Avantages :**
- Découplage émetteur/récepteur
- Chaîne configurable dynamiquement
- Responsabilité distribuée

### Command
**But :** Encapsule une requête dans un objet (permet annulation, historique).

**Utilisation :** Undo/Redo, macros, queues de commandes, transactions.

**Composants :**
- **Command** : interface commune
- **ConcreteCommand** : implémentation spécifique
- **Invoker** : lance les commandes
- **Receiver** : exécute l'action

### Interpreter
**But :** Définit une grammaire et un interprète pour exécuter des expressions.

**Utilisation :** Langages de script, expressions régulières, calculatrices, DSL.

**Structure :**
- Grammaire en forme d'arbre syntaxique
- Chaque règle = une classe
- Méthode `interpret()` pour l'évaluation

### Iterator
**But :** Accès séquentiel aux éléments d'une collection sans exposer sa structure.

**Utilisation :** Parcours de conteneurs, algorithmes génériques, abstraction des structures.

**Bénéfices :**
- Interface uniforme pour différentes collections
- Plusieurs itérateurs simultanés
- Découplage algorithme/structure

### Mediator
**But :** Centralise la communication entre plusieurs objets.

**Utilisation :** Interfaces complexes, réduction du couplage, coordination.

**Exemple :** Contrôleur aérien coordonnant les avions sans qu'ils communiquent directement.

### Strategy
**But :** Définit une famille d'algorithmes interchangeables dynamiquement.

**Utilisation :** Algorithmes de tri, compression, tarification, rendering.

**Avantages :**
- Élimination des conditionnelles
- Algorithmes configurables à l'exécution
- Facilite l'ajout de nouveaux algorithmes

### Template Method
**But :** Définit l'ossature d'un algorithme, laisse certaines étapes aux sous-classes.

**Utilisation :** Frameworks, algorithmes avec variantes, hooks personnalisables.

**Structure :**
```cpp
class AbstractAlgorithm {
public:
    void templateMethod() {
        step1();
        step2();  // implémentée par les sous-classes
        step3();
    }
protected:
    virtual void step2() = 0;
};
```

### Visitor
**But :** Sépare un algorithme de la structure sur laquelle il opère.

**Utilisation :** AST, sérialiseurs, processeurs de données, compilateurs.

**Avantages :**
- Ajouter de nouvelles opérations sans modifier les classes
- Regroupement d'opérations liées
- Utilisation du double dispatch

# 🔧 Concepts C++ avancés

## Templates variadiques
Permettent de passer un nombre variable d'arguments typés :
Ellipsis: `...`
Si tu mets `...` dans une fonction comme ceci: 

```cpp
template <typename... TArgs>
void func(TArgs... args) {
    (std::cout << ... << args) << std::endl; // C++17 fold expression
}

// Usage
func(1, "hello", 3.14);
```
Cela signifie que tu peux passer n'importe quel nombre d'arguments de n'importe quel type.

## Gestion des références et std::forward

- `T&` : référence lvalue uniquement
- `const T&` : référence constante (lvalues + rvalues)
- `T&&` : rvalue reference ou forwarding reference (en template)
- `std::forward<T>(arg)` : préserve la nature de l'argument

**Pourquoi std::forward ?**
- Évite les copies inutiles
- Préserve les optimisations de déplacement (move semantics)
- Permet le perfect forwarding

```cpp
template <typename... TArgs>
TType* acquire(TArgs&&... args) {
    void* memory = pool.allocate();
    return new (memory) TType(std::forward<TArgs>(args)...);
}
```

## std::aligned_storage

Template C++11 pour pré-allouer de la mémoire correctement alignée mais deprecated en c++23:

```cpp
std::aligned_storage<sizeof(T), alignof(T)>::type storage;
T* ptr = reinterpret_cast<T*>(&storage);
new (ptr) T(args...);  // Placement new
```

**Utilité :**
- Object pools
- Containers personnalisés
- Éviter les allocations dynamiques

## Default Noexcept Delete
Indique qu'une fonction ne lance pas d'exception, permettant des optimisations.

```cpp
void func() noexcept; // Garantie de ne pas lancer d'exception
```

Pour les constructeurs/destructeurs qui ne font rien dedans on peut les indiquer ` = default `

```cpp
class MyClass {
public:
    MyClass() = default; // Constructeur par défaut sans exception
    ~MyClass() = default; // Destructeur par défaut sans exception
};
```
Pour les constructeurs/destructeurs, `noexcept` est souvent implicite.

Si une methode ne doit jamais etre possible on peut aussi place r `= delete`

```cpp
class NonCopyable {
public:
    NonCopyable(const NonCopyable&) = delete; // Interdit la copie
    NonCopyable& operator=(const NonCopyable&) = delete; // Interdit la copie
};
```
## Liste des exceptions des plus courantes
```
std::exception
├── std::logic_error
│   ├── std::invalid_argument
│   ├── std::domain_error
│   ├── std::length_error
│   └── std::out_of_range
└── std::runtime_error
    ├── std::range_error
    ├── std::overflow_error
    └── std::underflow_error
```

# 🧪 Tests

Les tests utilisent **GoogleTest** (téléchargé automatiquement via CMake) et couvrent l'intégralité des composants de la librairie.

## Exécution des tests

```bash
# Compiler les tests
make gtest

# Lancer tous les tests GoogleTest
make run-gtest

# Sortie attendue
[==========] Running X tests from Y test suites.
[----------] Global test environment set-up.
...
[----------] Global test environment tear-down
[==========] X tests from Y test suites ran.
[  PASSED  ] X tests.
```

## Couverture des tests

**Structures de données :**
- ✅ `test_data_buffer.cpp` - Sérialisation/désérialisation
- ✅ `test_pool.cpp` - Allocation/libération mémoire
- ✅ `test_ring_buffer.cpp` - Buffer circulaire

**Design Patterns :**
- ✅ `test_memento.cpp` - Sauvegarde/restauration d'état
- ✅ `test_observer.cpp` - Pattern Observer classique
- ✅ `test_singleton.cpp` - Instance unique
- ✅ `test_state_machine.cpp` - Machine à états finis

**Threading :**
- ✅ `test_thread.cpp` - Wrapper de threads
- ✅ `test_thread_safe_queue.cpp` - Queue thread-safe
- ✅ `test_thread_safe_iostream.cpp` - IO thread-safe
- ✅ `test_worker_pool.cpp` - Pool de workers
- ✅ `test_persistent_worker.cpp` - Worker persistant

**Réseau :**
- ✅ `test_message.cpp` - Système de messages

**Mathématiques :**
- ✅ `test_ivector2.cpp` - Vecteurs 2D
- ✅ `test_ivector3.cpp` - Vecteurs 3D
- ✅ `test_perlin_noise.cpp` - Génération de bruit
- ✅ `test_random_2D_coordinate_generator.cpp` - Génération aléatoire

**Bonus :**
- ✅ `test_logger.cpp` - Système de logging
- ✅ `test_chronometre.cpp` - Mesure de temps
- ✅ `test_n_ary_tree.cpp` - Arbre n-aire
- ✅ `test_observable_value.cpp` - Valeurs observables

## Types de tests couverts

**Fonctionnalités de base :**
- Constructeurs/destructeurs
- Opérations CRUD (Create, Read, Update, Delete)
- Sérialisation/désérialisation

**Cas limites :**
- Buffers vides/pleins
- Valeurs nulles/invalides
- Dépassements de capacité

**Thread safety :**
- Accès concurrent
- Conditions de course
- Deadlocks potentiels

**Performance :**
- Complexité temporelle
- Fuites mémoire (via valgrind)
- Allocations inutiles

**Gestion d'erreurs :**
- Exceptions attendues
- États invalides
- Ressources épuisées

## Structure des tests


tests/
├── my_google_test/          # Tests GoogleTest (utilisés)
│   ├── main.cpp            # Point d'entrée des tests
│   ├── test_*.cpp          # Suites de tests par composant
│   └── ...
└── school_test/            # Tests manuels (non utilisés par make)
    ├── main_*.cpp          # Programmes de test individuels
    └── ...


## Exemple de test

```cpp
#include <gtest/gtest.h>
#include "libftpp.hpp"

TEST(DataBufferTest, SerializeAndDeserialize) {
    DataBuffer buffer;
    
    // Sérialisation
    buffer << 42 << 3.14f << std::string("test");
    
    // Désérialisation
    int i;
    float f;
    std::string s;
    buffer >> i >> f >> s;
    
    EXPECT_EQ(i, 42);
    EXPECT_FLOAT_EQ(f, 3.14f);
    EXPECT_EQ(s, "test");
}
```
