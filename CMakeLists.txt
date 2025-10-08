cmake_minimum_required(VERSION 3.14)
project(libftpp)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# C++11 et warnings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I. -Wall -Wextra -Werror")


# Bibliothèque de ton code - récupérer tous les .cpp automatiquement
file(GLOB_RECURSE LIB_SOURCES "src/**/*.cpp")
add_library(test_lib ${LIB_SOURCES})

# Ajouter l'exécutable de tests
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
add_executable(tests ${TEST_SOURCES})

# Ajouter l'exécutable de votre programme serveur
add_executable(program_server programs_test/programServer/main.cpp programServer/chuck.hpp)
target_link_libraries(program_server test_lib)
target_include_directories(program_server PRIVATE . src)


# Ajouter l'exécutable de votre programme serveur
add_executable(program_stress programs_test/programStressTesteur/main.cpp)
target_link_libraries(program_stress test_lib)
target_include_directories(program_stress PRIVATE . src)


# Lier GoogleTest et ta librairie
target_link_libraries(tests gtest_main test_lib)

# Inclure le dossier racine pour les includes et le dossier src
target_include_directories(tests PRIVATE . src)
target_include_directories(test_lib PRIVATE . src)

include (GoogleTest)
gtest_discover_tests(tests)