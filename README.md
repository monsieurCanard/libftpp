# libftpp
Further Exploration into Advanced C++

Your code must compile with at least the flag -std=c++11

Write class names in PascalCase format. Write method names in camelCase format. Files containing class code should always be named in snake_case according
to the class name. For instance:
class_name.hpp/class_name.h, class_name.cpp, or class_name.tpp. So, if you
have a header file containing the definition of a class “BrickWall” representing a
brick wall, the file name will be brick_wall.hpp

Unless specified otherwise, every output messages must end with a new-line character and be displayed to the standard output.

external libraries such as Boost are forbidden. The following functions are also prohibited: *printf(), *alloc(), and free().

 avoid memory leaks.

 Each of your headers should be usable independently of the others. Thus, they
must include all necessary dependencies. However, you must prevent the issue of
double inclusion by adding include guards

You must provide a unified header file named libftpp.hpp, which
includes all the necessary headers from your toolbox. This will enable other developers (or your future self) to easily integrate your library into new projects by
including just this single header file.

//DOCS

📦 Pool

Un Pool est un réservoir de mémoire qui pré-alloue un certain nombre d’objets et les réutilise.
Cela évite des allocations/désallocations fréquentes, ce qui améliore les performances, surtout si la création/destruction d’objets est coûteuse.
C’est une technique de design (non standard C++), mais très répandue.

🔧 Templates variadiques

template <typename... TArgs> : un template qui accepte un nombre variable d’arguments.

L’opérateur ... (ellipsis) sert à dérouler la liste d’arguments.

Exemple :
```cpp
template <typename... TArgs>
void func(TArgs... args) {
		(std::cout << ... << args) << std::endl; // Affiche tous les arguments
}
```
```cpp
pool.acquire(1, "test", 3.14); 
// Les paramètres sont transmis au constructeur de TType
```
🔗 Gestion des références

T& → référence classique (uniquement lvalues).

const T& → référence constante (accepte lvalues et rvalues).

T&& (hors template) → référence à rvalue uniquement.

T&& (dans un template) → forwarding reference (s’adapte selon l’argument).

📝 Boilerplate

Un terme qui désigne du code répétitif et standard, souvent nécessaire en C++
(par ex. pour gérer la mécanique des templates ou la mémoire).

🚀 Pourquoi utiliser std::forward ?

std::forward est recommandé car il préserve la nature des arguments (lvalue ou rvalue).

Sans forward :

Une rvalue est transformée en lvalue.

Cela provoque une copie inutile.

Certains constructeurs rvalue-only ne fonctionneront pas.

Avec forward :

Les rvalues sont transmises correctement.

On évite des copies coûteuses.

Les déplacements (move) sont possibles → meilleures performances.

Exemple :
```cpp
template <typename T>
void func(T&& arg) {
	std::cout << std::forward<T>(arg) << std::endl;
}
```
```cpp
template <typename... TArgs>
TType* acquire(TArgs&&... args) {
    void* memory = ...; // mémoire déjà réservée dans le Pool
    return new (memory) TType(std::forward<TArgs>(args)...);
}
```

td::aligned_storage

std::aligned_storage est un template struct fourni par le standard C++11 qui permet de pré-allouer de la mémoire brute suffisamment grande et correctement alignée pour contenir n’importe quel type T.

C’est très utile pour des object pools, le placement new, ou des constructions manuelles d’objets dans un buffer déjà alloué.

Syntaxe de base
```cpp
std::aligned_storage<sizeof(TType), alignof(TType)>::type myMemory;
```
sizeof(TType) : taille nécessaire pour stocker un objet de type TType

alignof(TType) : alignement requis pour le type TType

::type : récupère le type réel correspondant à la mémoire brute

On doit egalement toujours creer une fonction get qui retourne un pointeur vers TType en reinterpretant la mémoire brute.
