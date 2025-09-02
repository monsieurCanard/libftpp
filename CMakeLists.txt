cmake_minimum_required(VERSION 3.14)
project(libftpp)

# C++11 et warnings
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I. -Wall -Wextra -Werror")

# Ajouter le sous-module GoogleTest
add_subdirectory(extern/googletest)

# Bibliothèque de ton code - récupérer tous les .cpp automatiquement
file(GLOB_RECURSE LIB_SOURCES "srcs/**/*.cpp")
add_library(test_lib ${LIB_SOURCES})

# Ajouter l'exécutable de tests
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
add_executable(tests ${TEST_SOURCES})

# Lier GoogleTest et ta librairie
target_link_libraries(tests gtest_main test_lib)

# Inclure le dossier racine pour les includes et le dossier srcs
target_include_directories(tests PRIVATE . srcs)
target_include_directories(test_lib PRIVATE . srcs)