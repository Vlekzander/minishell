CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude -I$(LIBFT_DIR)/include -Itest/include -g
LDFLAGS = -L$(LIBFT_DIR) -lft -lreadline
LIBFT_DIR=libft
LIBFT_LIB=$(LIBFT_DIR)/libft.a
SOURCES_DIRS = src src/data src/error src/lexer src/parser src/utils
SOURCES_DIRS_TEST = $(wordlist 2,$(words $(SOURCES_DIRS)),$(SOURCES_DIRS)) test/src test/src/tests test/src/test_utils
SOURCES = $(foreach dir, $(SOURCES_DIRS), $(wildcard $(dir)/*.c))
SOURCES_TEST = $(foreach dir, $(SOURCES_DIRS_TEST), $(wildcard $(dir)/*.c))
OBJECTS = $(SOURCES:.c=.o)
OBJECTS_TEST = $(SOURCES_TEST:.c=.o)
DEPS = $(SOURCES:.c=.d)
DEPS_TEST = $(SOURCES_TEST:.c=.d)
NAME = minishell
NAME_TEST = minishell_tests

all: $(NAME) $(NAME_TEST)

$(NAME): $(OBJECTS)
	@echo "$(BLUE)✦ Building libraries...$(RESET)\r"
	@make -C $(LIBFT_DIR) --no-print-directory
	@printf "$(YELLOW)◈ Linking $@...$(RESET)\r"
	@$(CC) $^ $(LDFLAGS) -o $@
	@printf "$(GREEN)➤ Executable $(NAME) successfully built!$(RESET)\n"

test: $(NAME_TEST)
	@echo "$(BLUE)✦ Running unit tests...$(RESET)"
	@./$(NAME_TEST)

$(NAME_TEST): $(OBJECTS_TEST)
	@echo "$(BLUE)✦ Building libraries...$(RESET)\r"
	@make -C $(LIBFT_DIR) --no-print-directory
	@printf "$(YELLOW)◈ Linking $@...$(RESET)\r"
	@$(CC) $^ $(LDFLAGS) -o $@
	@printf "$(GREEN)➤ Executable $(NAME_TEST) successfully built!$(RESET)\n"

%.o: %.c
	@printf "$(YELLOW)◈ Compiling %s...$(RESET)\r" $<
	@$(CC) $(CFLAGS) -MMD -c $< -o $@
	@printf "$(GREEN)➤ Successfully compiled %s$(RESET)\n" $<

norm:
	@echo "$(BLUE)✦ Running norminette...$(RESET)"
	@norminette src include libft | grep Error || echo "$(GREEN)➤ No norms errors.$(RESET)"

clean:
	@echo "$(BLUE)✦ Cleaning object files...$(RESET)"
	@make -C $(LIBFT_DIR) clean --no-print-directory
	@rm -f $(OBJECTS) $(DEPS) $(OBJECTS_TEST) $(DEPS_TEST) && echo "$(GREEN)➤ Object files and dependencies removed.$(RESET)"

fclean: clean
	@echo "$(BLUE)✦ Cleaning executable...$(RESET)"
	@make -C $(LIBFT_DIR) fclean --no-print-directory
	@rm -f $(NAME) $(NAME_TEST) && echo "$(GREEN)➤ $(NAME) removed.$(RESET)"

re: fclean all

help:
	@echo "$(CYAN)Usage:$(RESET) make $(YELLOW)[target]$(RESET)"
	@echo ""
	@echo "$(BLUE)Targets:$(RESET)"
	@echo "  $(GREEN)all$(RESET)        : $(YELLOW)Build the project (default)$(RESET)"
	@echo "  $(GREEN)test$(RESET)       : $(YELLOW)Build and run tests$(RESET)"
	@echo "  $(GREEN)norm$(RESET)       : $(YELLOW)Run norminette$(RESET)"
	@echo "  $(GREEN)clean$(RESET)      : $(YELLOW)Remove object files$(RESET)"
	@echo "  $(GREEN)fclean$(RESET)     : $(YELLOW)Remove object files and the executable$(RESET)"
	@echo "  $(GREEN)re$(RESET)         : $(YELLOW)Rebuild the project (clean + all)$(RESET)"
	@echo "  $(GREEN)help$(RESET)       : $(YELLOW)Display this help message$(RESET)"

-include $(DEPS)

.PHONY: all norm clean fclean re help test

YELLOW = \033[1;33m
GREEN = \033[1;32m
BLUE = \033[1;34m
CYAN = \033[1;36m
RESET = \033[0m
