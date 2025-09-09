# libftpp

![C++](https://img.shields.io/badge/C%2B%2B-blue.svg)

## 📖 Description

**libftpp** est une librairie C++ moderne explorant les structures de données avancées et les design patterns. Ce projet met l'accent sur les bonnes pratiques du C++ moderne, la gestion efficace de la mémoire et l'implémentation rigoureuse des patterns classiques du génie logiciel.

## 🚀 Installation et compilation

### Prérequis
- CMake ≥ 3.14
- Compilateur C++11 ou supérieur

### Installation

```bash
git clone git@github.com:monsieurCanard/libftpp.git
cd libftpp
git submodule update --init --recursive
```

### Compilation
```bash
mkdir -p build
cd build
cmake ..
make
```

### Tests
```bash
cd build
./tests
```

## 📁 Structure du projet

```
libftpp/
├── srcs/
│   ├── data_structures/
│   │   ├── data_buffer/     # Sérialisation/désérialisation
│   │   └── pool/            # Pool de mémoire
│   └── design_patterns/
│       ├── memento/         # Sauvegarde d'état
│       ├── observer/        # Notification d'événements
│       ├── singleton/       # Instance unique
│       └── state_machine/   # Machine à états
├── tests/                   # Tests unitaires avec GoogleTest
├── extern/googletest/       # Sous-module GoogleTest
└── libftpp.hpp             # Header unifié
```

## 🎯 Règles de codage

- **Classes** : PascalCase (`MyClass`)
- **Méthodes** : camelCase (`myMethod()`)
- **Fichiers** : snake_case (`my_class.hpp`)
- **Standard** : C++11 minimum avec flags `-Wall -Wextra -Werror`
- **Interdictions** : Boost, `*printf()`, `*alloc()`, `free()`
- **Headers** : Indépendants avec include guards
- **Mémoire** : Éviter les fuites, gestion RAII (Resource Acquisition Is Initialization)

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


## 📚 Structures de données implementées

### 📦 Pool de mémoire

Un **Pool** est un réservoir de mémoire qui pré-alloue des objets pour éviter les allocations/désallocations fréquentes. Améliore significativement les performances pour la création d'objets coûteux.

**Caractéristiques :**
- Utilise `std::aligned_storage` pour l'alignement mémoire
- Templates variadiques pour les constructeurs
- Placement new pour la construction in-place

```cpp
Pool<MyClass> pool(100);  // Pré-alloue 100 objets
auto* obj = pool.acquire(arg1, arg2);  // Construction avec arguments
```

### 💾 DataBuffer

Système de **sérialisation/désérialisation** utilisant les streams C++ pour la persistance de données.

**Fonctionnalités :**
- Sérialisation via `reinterpret_cast<unsigned int*>` pour copier les octets
- Désérialisation via `memcpy` pour restaurer les données


## 🧵 Programmation concurrente

### 🔄 Condition Variables

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

## 🎨 Details - Design Patterns (✅ = implémentés)

### 🔹 Création (Creational Patterns)

#### ✅ Singleton
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

#### Factory Method
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

#### Abstract Factory
**But :** Fournit une interface pour créer des familles d'objets liés sans spécifier leurs classes concrètes.
**Utilisation :** Systèmes multi-plateformes, thèmes d'interface, familles de produits.

**Avantages :**
- Isole les classes concrètes
- Facilite l'échange de familles de produits
- Garantit la cohérence entre produits

#### Builder
**But :** Sépare la construction complexe d'un objet de sa représentation finale.

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

#### Prototype
**But :** Crée de nouveaux objets en copiant un prototype existant.

**Utilisation :** Clonage d'objets complexes, éviter la re-initialisation coûteuse.

### 🔹 Structure (Structural Patterns)

#### Adapter
**But :** Convertit l'interface d'une classe en une autre attendue par le client.

**Utilisation :** Intégration de librairies tierces, legacy code, APIs incompatibles.

**Types :**
- **Object Adapter** : utilise la composition
- **Class Adapter** : utilise l'héritage multiple

#### Bridge
**But :** Sépare l'abstraction de son implémentation pour les faire évoluer indépendamment.

**Utilisation :** Éviter l'explosion combinatoire d'héritages, architectures multi-plateformes.

**Avantages :**
- Découplage abstraction/implémentation
- Extensibilité facilitée
- Masquage des détails d'implémentation

#### Composite
**But :** Permet de traiter un groupe d'objets comme un objet unique (structure d'arbre).

**Utilisation :** Systèmes de fichiers, interfaces graphiques, menus hiérarchiques.

**Caractéristiques :**
- Structure récursive
- Traitement uniforme des objets simples et composites
- Facilite l'ajout de nouveaux types de composants

#### Decorator
**But :** Ajoute dynamiquement des responsabilités à un objet sans modifier sa classe.

**Utilisation :** Extensions de fonctionnalités, middleware, streams, GUI.

**Avantages :**
- Alternative flexible à l'héritage
- Combinaison dynamique de comportements
- Respect du principe Open/Closed

#### Facade
**But :** Fournit une interface simplifiée à un ensemble complexe de classes/sous-systèmes.

**Utilisation :** APIs simplifiées, masquer la complexité interne, point d'entrée unifié.

**Bénéfices :**
- Réduction du couplage
- Interface plus simple
- Centralisation des interactions

#### Flyweight
**But :** Partage les objets pour économiser mémoire/performances.

**Utilisation :** Nombreuses instances d'objets similaires (caractères, particules, tuiles).

**Concepts clés :**
- **État intrinsèque** : partagé entre instances
- **État extrinsèque** : unique à chaque contexte
- Factory pour gérer les instances partagées

#### Proxy
**But :** Fournit un substitut qui contrôle l'accès à un objet réel.

**Utilisation :** Lazy loading, contrôle d'accès, cache, logging, réseaux.

**Types de proxy :**
- **Virtual Proxy** : création à la demande
- **Protection Proxy** : contrôle d'accès
- **Remote Proxy** : représentant local d'un objet distant

### 🔹 Comportement (Behavioral Patterns)

#### ✅ Observer
**But :** Notifie automatiquement une liste d'observateurs lors d'un changement d'état.

**Utilisation :** Interfaces utilisateur, événements système, MVC, reactive programming.

**Avantages :**
- Découplage entre sujet et observateurs
- Communication broadcast
- Support dynamique d'observateurs

**Implémentation typique :**
```cpp
class Subject {
    std::vector<Observer*> observers;
public:
    void attach(Observer* obs) { observers.push_back(obs); }
    void notify() { for(auto* obs : observers) obs->update(); }
};
```

#### ✅ Memento
**But :** Sauvegarde/restaure l'état interne d'un objet sans violer l'encapsulation.

**Utilisation :** Undo/Redo, snapshots, sauvegarde d'état, checkpoints.

**Principe :** Externalise l'état sans exposer la structure interne.

**Structure :**
- **Originator** : crée et utilise les mementos
- **Memento** : stocke l'état interne
- **Caretaker** : gère les mementos sans les examiner

#### ✅ State Machine
**But :** Change le comportement d'un objet selon son état interne.

**Utilisation :** Parsers, protocoles réseau, jeux (IA), workflows, automates.

**Avantages :**
- Code plus maintenable pour les logiques d'état complexes
- Élimination des conditions if/switch complexes
- États explicites et transitions claires

**Implémentation :**
```cpp
class StateMachine {
    State* currentState;
public:
    void transition(State* newState) {
        currentState->exit();
        currentState = newState;
        currentState->enter();
    }
};
```

#### Chain of Responsibility
**But :** Passe une requête à travers une chaîne d'objets susceptibles de la traiter.

**Utilisation :** Validation, middleware, gestionnaires d'événements, parsers.

**Avantages :**
- Découplage émetteur/récepteur
- Chaîne configurable dynamiquement
- Responsabilité distribuée

#### Command
**But :** Encapsule une requête dans un objet (permet annulation, historique).

**Utilisation :** Undo/Redo, macros, queues de commandes, transactions.

**Composants :**
- **Command** : interface commune
- **ConcreteCommand** : implémentation spécifique
- **Invoker** : lance les commandes
- **Receiver** : exécute l'action

#### Interpreter
**But :** Définit une grammaire et un interprète pour exécuter des expressions.

**Utilisation :** Langages de script, expressions régulières, calculatrices, DSL.

**Structure :**
- Grammaire en forme d'arbre syntaxique
- Chaque règle = une classe
- Méthode `interpret()` pour l'évaluation

#### Iterator
**But :** Accès séquentiel aux éléments d'une collection sans exposer sa structure.

**Utilisation :** Parcours de conteneurs, algorithmes génériques, abstraction des structures.

**Bénéfices :**
- Interface uniforme pour différentes collections
- Plusieurs itérateurs simultanés
- Découplage algorithme/structure

#### Mediator
**But :** Centralise la communication entre plusieurs objets.

**Utilisation :** Interfaces complexes, réduction du couplage, coordination.

**Exemple :** Contrôleur aérien coordonnant les avions sans qu'ils communiquent directement.

#### Strategy
**But :** Définit une famille d'algorithmes interchangeables dynamiquement.

**Utilisation :** Algorithmes de tri, compression, tarification, rendering.

**Avantages :**
- Élimination des conditionnelles
- Algorithmes configurables à l'exécution
- Facilite l'ajout de nouveaux algorithmes

#### Template Method
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

#### Visitor
**But :** Sépare un algorithme de la structure sur laquelle il opère.

**Utilisation :** AST, sérialiseurs, processeurs de données, compilateurs.

**Avantages :**
- Ajouter de nouvelles opérations sans modifier les classes
- Regroupement d'opérations liées
- Utilisation du double dispatch

## 🔧 Concepts C++ avancés

### Templates variadiques
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

### Gestion des références et std::forward

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

### std::aligned_storage

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


## 🧪 Tests

Les tests utilisent **GoogleTest** et couvrent :
- ✅ Fonctionnalités des structures de données
- ✅ Comportement des design patterns
- ✅ Cas limites et gestion d'erreurs
- ✅ Performance et fuites mémoire

**Structure des tests :**
```
tests/
├── test_data_buffer.cpp
├── test_pool.cpp
├── test_memento.cpp
├── test_observer.cpp
├── test_singleton.cpp
└── test_state_machine.cpp
```
