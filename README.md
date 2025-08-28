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

POOL :
Un Pool c’est une sorte de “réservoir” de mémoire qui pré-alloue un certain nombre d’objets et les réutilise pour éviter des allocations/désallocations fréquentes.

... (ellipsis) en C++ sert à “dérouler” ta liste d’arguments (ou de types) dans un variadic template.

Un template Variadic est un template qui peut prendre un nombre variable d’arguments (ou de types). C’est utile pour créer des classes ou des fonctions génériques qui peuvent gérer différents types ou nombres d’arguments.

T& = référence classique (uniquement lvalues).

const T& = référence constante (lvalues et rvalues).

T&& (non-template) = référence à rvalue seulement.

T&& (dans un template) = forwarding reference (s’adapte).

Boilerplate = du code répétitif et standard

acquire (et plus largement les object pools) ne sont pas du C++ standard.
C’est une technique classique de design


pourquoi std::forward est recommandé

Préserve la nature lvalue/rvalue

Si l’utilisateur passe une rvalue (ex : pool.acquire(42)), std::forward transmet la rvalue au constructeur de TType.

Sans forward, toutes les rvalues deviennent des lvalues, donc :

Il y a une copie inutile au lieu d’un déplacement.

Certains constructeurs rvalue-only ne fonctionneront pas.

Performance

forward permet de ne pas créer de copies inutiles, donc c’est plus performant, surtout si TType est lourd à copier.