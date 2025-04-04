/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tests.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 16:34:22 by apierret          #+#    #+#             */
/*   Updated: 2025/04/04 12:34:23 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"
#include "lexer.h"
#include "tests.h"
#include "test_utils.h"

static t_case token_cases[] = {
	{ "single_command",  { { "pwd", { "pwd", NULL } } } },
	{ "one_arg",     { { "echo hello", { "echo", "hello", NULL } } } },
	{ "multiple_args", { { "echo hello world", { "echo", "hello", "world", NULL } } } },
	{ "redir_out", { { "echo hello > out.txt", { "echo", "hello", ">", "out.txt", NULL } } } },
	{ "redir_append", { { "echo world >> out.txt", { "echo", "world", ">>", "out.txt", NULL } } } },
	{ "redir_in", { { "cat < in.txt", { "cat", "<", "in.txt", NULL } } } },
	{ "redir_heredoc", { { "cat << END", { "cat", "<<", "END", NULL } } } },
	{ "redir_out_inverted", { { "> out.txt echo hello", { ">", "out.txt", "echo", "hello", NULL } } } },
	{ "redir_append_inverted", { { ">> out.txt echo world", { ">>", "out.txt", "echo", "world", NULL } } } },
	{ "redir_in_inverted", { { "< in.txt cat", { "<", "in.txt", "cat", NULL } } } },
	{ "redir_heredoc_inverted", { { "<< END cat", { "<<", "END", "cat", NULL } } } },
	{ "single_pipe", { { "ls | grep .c", { "ls", "|", "grep", ".c", NULL } } } },
	{ "multiple_pipes", { { "ls | grep .c | wc -l", { "ls", "|", "grep", ".c", "|", "wc", "-l", NULL } } } },
	{ "pipes_and_redir", { { "cat < in.txt | grep hello | wc -l > out.txt", { "cat", "<", "in.txt", "|", "grep", "hello", "|", "wc", "-l", ">", "out.txt", NULL } } } },
	{ "quoted_arg",      { { "echo \"hello world\"", { "echo", "hello world", NULL } } } },
	{ "mixed_quotes",    { { "echo \"it's fine\"", { "echo", "it's fine", NULL } } } },
	{ "command_and",     { { "make && ./program", { "make", "&&", "./program", NULL } } } },
	{ "command_or",      { { "./build || echo \"fail\"", { "./build", "||", "echo", "fail", NULL } } } },
	{ "subshell",        { { "(pwd)", { "(", "pwd", ")", NULL } } } },
	{ "and_with_redir", { { "echo success > out.txt && cat < in.txt", { "echo", "success", ">", "out.txt", "&&", "cat", "<", "in.txt", NULL } } } },
	{ "or_with_pipe", { { "false || ls | grep .h", { "false", "||", "ls", "|", "grep", ".h", NULL } } } },
	{ "subshell_with_pipe", { { "(ls -la) | wc -l", { "(", "ls", "-la", ")", "|", "wc", "-l", NULL } } } },
	{ "subshell_and_redir", { { "(echo hello) > out.txt && echo done", { "(", "echo", "hello", ")", ">", "out.txt", "&&", "echo", "done", NULL } } } },
	{ "nested_logic", { { "make && (./run || echo \"fallback\")", { "make", "&&", "(", "./run", "||", "echo", "fallback", ")", NULL } } } },
	{ "deep_nested_group", { { "(((echo ok)))", { "(", "(", "(", "echo", "ok", ")", ")", ")", NULL } } } },
	{ "multi_op_complex", { { "(cat file | grep error) && echo \"found\" || echo \"none\"", { "(", "cat", "file", "|", "grep", "error", ")", "&&", "echo", "found", "||", "echo", "none", NULL } } } },

	{ NULL }
};

MunitResult	tokenize_basic_tests(const MunitParameter params[], void* data)
{
	t_case	*tc;
	t_list	*excepted;
	t_list	*tested;
	int		equal;

	(void) data;
	tc = find_case(token_cases, munit_parameters_get(params, "case"));
	excepted = NULL;
	tested = NULL;
	if (tc == NULL)
		return (munit_log(MUNIT_LOG_ERROR, "Test case not found"), MUNIT_ERROR);
	excepted = create_token_list(tc->expected_tokens);
	tokenize(&tested, tc->input);
	equal = lst_equal(excepted, tested, token_equal);
	ft_lstclear(&tested, (void (*)(void *)) free_token);
	ft_lstclear(&excepted, (void (*)(void *)) free_token);
	if (!equal)
		return (MUNIT_FAIL);
	return (MUNIT_OK);
}