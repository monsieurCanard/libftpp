NAME = libftpp.a

CFLAGS = -std=c++17 -Wall -Wextra -Werror

SRCS_DIR = src/

DATA_STRUCTURES_DIR = $(SRCS_DIR)data_structures/
DESIGN_PATTERNS_DIR = $(SRCS_DIR)design_patterns/
NETWORK_DIR = $(SRCS_DIR)network/
MATHEMATICS_DIR = $(SRCS_DIR)mathematics/
THREAD_DIR = $(SRCS_DIR)thread/
BONUS_DIR = $(SRCS_DIR)bonus/

SRCS = $(SRCS_DIR)data_structures/data_buffer/data_buffer.cpp \
			 $(SRCS_DIR)design_patterns/memento/memento.cpp \
			 $(SRCS_DIR)network/message/message.cpp \
			 $(SRCS_DIR)network/server/server.cpp \
			 $(SRCS_DIR)network/client/client.cpp \
			 $(SRCS_DIR)mathematics/perlin_noise/perlin_noise.cpp \
			 $(SRCS_DIR)mathematics/random_2D_coordinate_generator/random_2D_coordinate_generator.cpp \
			 $(SRCS_DIR)thread/thread_safe_iostream/thread_safe_iostream.cpp \
			 $(SRCS_DIR)thread/persistent_worker/persistent_worker.cpp \
			 $(SRCS_DIR)thread/thread/thread.cpp \
			 $(SRCS_DIR)thread/worker_pool/worker_pool.cpp \
			 $(SRCS_DIR)bonus/logger/logger.cpp \
			 $(SRCS_DIR)bonus/chronometre/chronometre.cpp \
			 $(SRCS_DIR)bonus/ring_buffer/ring_buffer.cpp

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
	@echo "\033[0;34m[✔] Compiled $<\
	\033[0m"

SRCS_TEST = $(shell find tests/ -name "*.cpp")

test: $(SRCS_TEST) $(NAME)
	if [ ! -d "build/Makefile" ]; then mkdir -p build; fi
	mkdir -p build
	cd build && cmake .. && make
	@echo "\033[0;32m[✔] Tests compiled.\033[0m"

run-test: test
	./build/tests

clean: fclean all

fclean:
	rm -rf $(OBJ_DIR)
	rm -f $(NAME)
	@echo "\033[0;31m[✘] Removed objects and $(NAME).\
	\033[0m"

re: fclean all