CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude -I$(LIBFT_DIR)/include -Itest/include -g
LDFLAGS = -L$(LIBFT_DIR) -lft -lreadline
LIBFT_DIR=libft
LIBFT_LIB=$(LIBFT_DIR)/libft.a
SOURCES_DIRS = src src/data src/env src/error src/lexer src/parser src/redirs src/utils
SOURCES = $(foreach dir, $(SOURCES_DIRS), $(wildcard $(dir)/*.c))
OBJECTS = $(SOURCES:.c=.o)
DEPS = $(SOURCES:.c=.d)
NAME = minishell
SOURCES_DIRS_TEST = $(wordlist 2,$(words $(SOURCES_DIRS)),$(SOURCES_DIRS)) test/src test/src/test_utils
SOURCES_TEST = $(foreach dir, $(SOURCES_DIRS_TEST), $(wildcard $(dir)/*.c))
OBJECTS_TEST = $(SOURCES_TEST:.c=.o)
TEST_VARIANTS_DIR = test/src/tests
TEST_VARIANTS = $(wildcard $(TEST_VARIANTS_DIR)/*.c)
TEST_VARIANT_OBJECTS = $(TEST_VARIANTS:.c=.o)
TEST_BINS = $(patsubst $(TEST_VARIANTS_DIR)/%.c, bin_test/%, $(TEST_VARIANTS))
DEPS_TEST = $(SOURCES_TEST:.c=.d) $(TEST_VARIANTS:.c=.d)

wraps_tokenize_tests = -Wl,--wrap=expand
wraps_expand_tests = -Wl,-wrap=extract_pattern,-wrap=globbing,-wrap=scan_dir,-wrap=extract_var

all: $(NAME) $(TEST_BINS)

$(NAME): $(OBJECTS)
	@echo "$(BLUE)✦ Building libraries...$(RESET)\r"
	@make -C $(LIBFT_DIR) --no-print-directory
	@printf "$(YELLOW)◈ Linking $@...$(RESET)\r"
	@$(CC) $^ $(LDFLAGS) -o $@
	@printf "$(GREEN)➤ Executable $(NAME) successfully built!$(RESET)\n"

test: $(TEST_BINS)
	@printf "[ $(GREEN)TESTS$(RESET) ] Running...\n"
	@fails=0; \
	for exe in $(TEST_BINS); do \
		./$$exe || fails=$$((fails + $$?)); \
	done; \
	printf "[ $(YELLOW)RESULTS$(RESET) ] Tests failed: $$fails\n";

bin_test/%: $(TEST_VARIANTS_DIR)/%.o $(OBJECTS_TEST)
	@mkdir -p bin_test
	@echo "$(BLUE)✦ Building libraries...$(RESET)\r"
	@make -C $(LIBFT_DIR) --no-print-directory
	@printf "$(YELLOW)◈ Linking $@...$(RESET)\r"
	@$(CC) $(TEST_CFLAGS) $< $(OBJECTS_TEST) $(LDFLAGS) -lcmocka $(if $(value wraps_$*),$(wraps_$*)) -o $@
	@printf "$(GREEN)➤ Executable $@ successfully built!$(RESET)\n"

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
	@rm -f $(OBJECTS) $(OBJECTS_TEST) $(TEST_VARIANT_OBJECTS) $(DEPS) $(DEPS_TEST) && echo "$(GREEN)➤ Object files and dependencies removed.$(RESET)"

fclean: clean
	@echo "$(BLUE)✦ Cleaning executable...$(RESET)"
	@make -C $(LIBFT_DIR) fclean --no-print-directory
	@rm -f $(NAME) && echo "$(GREEN)➤ $(NAME) removed.$(RESET)"
	@rm -rf bin_test && echo "$(GREEN)➤ test binaries removed.$(RESET)"

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

-include $(DEPS) $(DEPS_TEST)

.PHONY: all norm clean fclean re help test

YELLOW = \033[1;33m
GREEN = \033[1;32m
BLUE = \033[1;34m
CYAN = \033[1;36m
RESET = \033[0m