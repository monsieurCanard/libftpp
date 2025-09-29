NAME = libftpp.a

CFLAGS = -std=c++11 -Wall -Wextra -Werror -I

# CFLAGS_DEBUG = -std=c++11 -DDEBUG -Wall -Wextra -Werror

SRCS_DIR = src/

DATA_STRUCTURES_DIR = $(SRCS_DIR)data_structures/
DESIGN_PATTERNS_DIR = $(SRCS_DIR)design_patterns/
NETWORK_DIR = $(SRCS_DIR)network/
MATHEMATICS_DIR = $(SRCS_DIR)mathematics/
THREAD_DIR = $(SRCS_DIR)thread/
BONUS_DIR = $(SRCS_DIR)bonus/

SRCS = $(shell find $(SRCS_DIR) -name "*.cpp")

INCLUDE_DIRS = -I$(SRCS_DIR) \
               -I$(DATA_STRUCTURES_DIR) \
               -I$(DESIGN_PATTERNS_DIR) \
               -I$(NETWORK_DIR) \
               -I$(MATHEMATICS_DIR) \
               -I$(THREAD_DIR) \
               -I$(BONUS_DIR)

OBJ_DIR = src/objs/

OBJS = $(patsubst $(SRCS_DIR)%.cpp, $(OBJ_DIR)%.o, $(SRCS))

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $@ $^
	@echo "\033[0;32m[✔] Library $(NAME) created.\
	\033[0m"

$(OBJ_DIR):
	mkdir -p $@


$(OBJ_DIR)%.o: $(SRCS_DIR)%.cpp | $(OBJ_DIR)
	g++ $(CFLAGS) -I$(INCLUDE_DIRS) -c $< -o $@
	echo "\033[0;34m[✔] Compiled $<\
	\033[0m"

clean: fclean all

fclean:
	rm -rf $(OBJ_DIR)
	rm -f $(NAME)
	@echo "\033[0;31m[✘] Removed objects and $(NAME).\
	\033[0m"

re: fclean all