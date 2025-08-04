CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude -I$(LIBFT_DIR)/include -g
LDFLAGS = -L$(LIBFT_DIR) -lft -lreadline
LIBFT_DIR=libft
LIBFT_LIB=$(LIBFT_DIR)/libft.a
SOURCES_DIRS = src src/builtins src/data src/env src/error src/executor src/expansion src/hash src/lexer src/parser src/redirs src/signals src/utils
SOURCES =	src/main.c	\
			src/builtins/builtin_cd.c src/builtins/builtin_echo.c src/builtins/builtin_env.c src/builtins/builtin_exit.c src/builtins/builtin_export.c src/builtins/builtin_pwd.c src/builtins/builtin_unset.c src/builtins/get_builtin.c	\
			src/data/create_ast.c src/data/create_pattern.c src/data/create_redir.c src/data/create_strbuilder.c src/data/create_token.c src/data/free_ast.c src/data/free_command.c src/data/free_pattern.c src/data/free_redir.c src/data/free_strbuilder.c src/data/free_token.c src/data/get_token_type.c src/data/strbuilder_append.c src/data/strbuilder_append_char.c src/data/strbuilder_append_mchar.c	\
			src/env/export_char_order.c src/env/get_env.c src/env/get_env_str.c src/env/get_var.c src/env/load_env.c src/env/remove_var.c src/env/set_var.c	\
			src/error/error.c src/error/errorq.c src/error/print_error.c	\
			src/executor/execute_command.c src/executor/execute_command_node.c src/executor/execute_logic_node.c src/executor/execute_node.c src/executor/execute_pipeline_node.c src/executor/execute_redir_node.c src/executor/find_executable.c src/executor/get_exit_code.c src/executor/in_pipeline.c src/executor/prepare_command.c	\
			src/expansion/create_mask.c src/expansion/expand_list.c src/expansion/expand_redir_target.c src/expansion/extract_pattern.c src/expansion/extract_vars.c src/expansion/globbing.c src/expansion/pathname_expand.c src/expansion/pathname_expansion.c src/expansion/quote_removal.c src/expansion/substitute_vars.c src/expansion/variable_expansion.c src/expansion/word_splitting.c	\
			src/hash/create_hnode.c src/hash/free_hnode.c src/hash/htable_destroy.c src/hash/htable_foreach.c src/hash/htable_get.c src/hash/htable_hash.c src/hash/htable_init.c src/hash/htable_insert.c src/hash/htable_remove.c src/hash/htable_search_node.c	\
			src/lexer/lexer_utils.c src/lexer/process_input.c src/lexer/tokenize.c src/lexer/tokenize_process.c	\
			src/parser/led_process.c src/parser/nud_process.c src/parser/parse_ast.c src/parser/parse_utils.c src/parser/redir_cond.c src/parser/redir_to_list.c	\
			src/redirs/close_redirs.c src/redirs/handle_redirs.c src/redirs/hd_prepare_eof.c src/redirs/open_file.c src/redirs/prepare_redirs.c src/redirs/process_heredoc.c src/redirs/prompt_redirs.c	\
			src/signals/restore_signals.c src/signals/setup_signals.c	\
			src/utils/check_file.c src/utils/close_utils.c src/utils/fill_buffer_random.c src/utils/free_ddarray.c src/utils/ft_strndup.c src/utils/ft_strstr.c src/utils/insert_sublist.c src/utils/is_blank.c src/utils/lst_array.c src/utils/lst_remove.c src/utils/lst_sort.c src/utils/quote_utils.c src/utils/remove_str_quotes.c src/utils/scan_dir.c src/utils/str_conditions.c
OBJECTS = $(SOURCES:.c=.o)
DEPS = $(SOURCES:.c=.d)
NAME = minishell

all: $(NAME)

$(NAME): $(OBJECTS)
	@echo "$(BLUE)✦ Building libraries...$(RESET)\r"
	@make -C $(LIBFT_DIR) --no-print-directory
	@printf "$(YELLOW)◈ Linking $@...$(RESET)\r"
	@$(CC) $^ $(LDFLAGS) -o $@
	@printf "$(GREEN)➤ Executable $(NAME) successfully built!$(RESET)\n"

.c.o:
	@printf "$(YELLOW)◈ Compiling %s...$(RESET)\r" $<
	@$(CC) $(CFLAGS) -MMD -c $< -o $@
	@printf "$(GREEN)➤ Successfully compiled %s$(RESET)\n" $<

valgrind: $(NAME)
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --trace-children=yes --suppressions=readline.supp -s ./$(NAME)

norm:
	@echo "$(BLUE)✦ Running norminette...$(RESET)"
	@norminette src include libft | grep Error || echo "$(GREEN)➤ No norms errors.$(RESET)"

clean:
	@echo "$(BLUE)✦ Cleaning object files...$(RESET)"
	@make -C $(LIBFT_DIR) clean --no-print-directory
	@rm -f $(OBJECTS) $(DEPS) && echo "$(GREEN)➤ Object files and dependencies removed.$(RESET)"

fclean: clean
	@echo "$(BLUE)✦ Cleaning executable...$(RESET)"
	@make -C $(LIBFT_DIR) fclean --no-print-directory
	@rm -f $(NAME) && echo "$(GREEN)➤ $(NAME) removed.$(RESET)"

re: fclean all

help:
	@echo "$(CYAN)Usage:$(RESET) make $(YELLOW)[target]$(RESET)"
	@echo ""
	@echo "$(BLUE)Targets:$(RESET)"
	@echo "  $(GREEN)all$(RESET)        : $(YELLOW)Build the project (default)$(RESET)"
	@echo "  $(GREEN)valgrind$(RESET)   : $(YELLOW)Run project with valgrind$(RESET)"
	@echo "  $(GREEN)norm$(RESET)       : $(YELLOW)Run norminette$(RESET)"
	@echo "  $(GREEN)clean$(RESET)      : $(YELLOW)Remove object files$(RESET)"
	@echo "  $(GREEN)fclean$(RESET)     : $(YELLOW)Remove object files and the executable$(RESET)"
	@echo "  $(GREEN)re$(RESET)         : $(YELLOW)Rebuild the project (clean + all)$(RESET)"
	@echo "  $(GREEN)help$(RESET)       : $(YELLOW)Display this help message$(RESET)"

-include $(DEPS)

.PHONY: all norm clean fclean re help valgrind

YELLOW = \033[1;33m
GREEN = \033[1;32m
BLUE = \033[1;34m
CYAN = \033[1;36m
RESET = \033[0m
