cmake_minimum_required(VERSION 3.14)
project(libftpp)

# C++11 et warnings
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

# Ajouter le sous-module GoogleTest
add_subdirectory(extern/googletest)

# Bibliothèque de ton code
add_library(test_lib 
srcs/data_structures/data_buffer/data_buffer.cpp
srcs/design_patterns/memento/memento.cpp
srcs/design_patterns/thread_safe_iostream/thread_safe_iostream.cpp
)

# Ajouter l'exécutable de tests
add_executable(tests 
tests/main.cpp
tests/test_data_buffer.cpp
tests/test_pool.cpp
tests/test_memento.cpp
tests/test_observer.cpp
tests/test_singleton.cpp
tests/test_state_machine.cpp
tests/test_thread_safe_iostream.cpp
)

# Lier GoogleTest et ta librairie
target_link_libraries(tests gtest_main test_lib)

# Inclure les headers de ton projet
target_include_directories(tests PRIVATE srcs srcs/data_structures/data_buffer
srcs/data_structures/pool
srcs/design_patterns/memento
srcs/design_patterns/observer
srcs/design_patterns/singleton
srcs/design_patterns/state_machine
srcs/design_patterns/thread_safe_iostream)
