NAME = libftpp.a

CFLAGS = -std=c++17 -Wall -Wextra -Werror

SRCS_DIR = src/

DATA_STRUCTURES_DIR = $(SRCS_DIR)data_structures/
DESIGN_PATTERNS_DIR = $(SRCS_DIR)design_patterns/
NETWORK_DIR = $(SRCS_DIR)network/
MATHEMATICS_DIR = $(SRCS_DIR)mathematics/
THREAD_DIR = $(SRCS_DIR)thread/
BONUS_DIR = $(SRCS_DIR)bonus/

SRCS = $(DATA_STRUCTURES_DIR)/data_buffer/data_buffer.cpp \
			 $(DESIGN_PATTERNS_DIR)memento/memento.cpp \
			 $(NETWORK_DIR)message/message.cpp \
			 $(NETWORK_DIR)server/server.cpp \
			 $(NETWORK_DIR)client/client.cpp \
			 $(MATHEMATICS_DIR)perlin_noise_2D/perlin_noise_2D.cpp \
			 $(MATHEMATICS_DIR)random_2D_coordinate_generator/random_2D_coordinate_generator.cpp \
			 $(THREAD_DIR)thread_safe_iostream/thread_safe_iostream.cpp \
			 $(THREAD_DIR)persistent_worker/persistent_worker.cpp \
			 $(THREAD_DIR)thread/thread.cpp \
			 $(THREAD_DIR)worker_pool/worker_pool.cpp \
			 $(BONUS_DIR)logger/logger.cpp \
			 $(BONUS_DIR)chronometre/chronometre.cpp \
			 $(BONUS_DIR)ring_buffer/ring_buffer.cpp

INCLUDE_DIRS = -I$(SRCS_DIR) \
               -I$(DATA_STRUCTURES_DIR) \
               -I$(DESIGN_PATTERNS_DIR) \
               -I$(NETWORK_DIR) \
               -I$(MATHEMATICS_DIR) \
               -I$(THREAD_DIR) \
               -I$(BONUS_DIR)


OBJ_DIR = objs/

OBJS = $(SRCS:$(SRCS_DIR)%.cpp=$(OBJ_DIR)%.o)

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $@ $^
	@echo "\033[0;32m[✔] Library $(NAME) created.\
	\033[0m"

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)%.o: $(SRCS_DIR)%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	g++ $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@

CMAKE_FILES = CMakeLists.txt

TEST_BINARY = build/tests

TEST_SRCS = $(wildcard tests/my_google_test/*.cpp)

build/Makefile: $(CMAKE_FILES) 
	mkdir -p build
	cd build && cmake ..

$(TEST_BINARY): $(NAME) build/Makefile $(TEST_SRCS)
	cd build && make

gtest: $(TEST_BINARY)

run-gtest:
	./build/tests

run-42-test:
	./tests/launch_test.sh

clean:
	rm -rf $(OBJ_DIR)

fclean:
	rm -rf $(OBJ_DIR)
	rm -rf tests/main_client_test
	rm -rf tests/main_server_test
	rm -f $(NAME)
	rm -rf build
	rm -f save.txt
	rm -rf logs

re: fclean all

.PHONY : all clean fclean re test run-test