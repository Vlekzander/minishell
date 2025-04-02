/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tests.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 16:34:22 by apierret          #+#    #+#             */
/*   Updated: 2025/04/02 16:37:18 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"
#include "lexer.h"
#include "tests.h"
#include "test_utils.h"

static t_case token_cases[] = {
	{ "echo_simple",     { { "echo hello", { "echo", "hello", NULL } } } },
	{ "single_command",  { { "pwd", { "pwd", NULL } } } },
	{ "command_with_flag", { { "ls -a", { "ls", "-a", NULL } } } },
	{ "redir_output",    { { "echo hello > out.txt", { "echo", "hello", ">", "out.txt", NULL } } } },
	{ "redir_input",     { { "cat < file.txt", { "cat", "<", "file.txt", NULL } } } },
	{ "append_redir",    { { "echo hi >> log.txt", { "echo", "hi", ">>", "log.txt", NULL } } } },
	{ "heredoc",         { { "cat << EOF", { "cat", "<<", "EOF", NULL } } } },
	{ "pipe_simple",     { { "ls | grep .c", { "ls", "|", "grep", ".c", NULL } } } },
	{ "pipe_chain",      { { "cat file | sort | uniq", { "cat", "file", "|", "sort", "|", "uniq", NULL } } } },
	{ "quoted_arg",      { { "echo \"hello world\"", { "echo", "hello world", NULL } } } },
	{ "mixed_quotes",    { { "echo \"it's fine\"", { "echo", "it's fine", NULL } } } },
	{ "command_and",     { { "make && ./program", { "make", "&&", "./program", NULL } } } },
	{ "command_or",      { { "./build || echo \"fail\"", { "./build", "||", "echo", "fail", NULL } } } },
	{ "subshell",        { { "(pwd)", { "(", "pwd", ")", NULL } } } },
	{ NULL }
};

MunitResult	tokenize_basic_tests(const MunitParameter params[], void* data)
{
	(void) data;
	t_case	*tc = find_case(token_cases, munit_parameters_get(params, "case"));
	t_list		*excepted = NULL;
	t_list		*tested = NULL;
	t_error		result;

	if (tc == NULL)
		return (munit_log(MUNIT_LOG_ERROR, "Test case not found"), MUNIT_ERROR);
	excepted = create_token_list(tc->expected_tokens);
	result = tokenize(&tested, tc->input);
	munit_assert_true(lst_equal(excepted, tested, token_equal));
	munit_assert_true(result == ERR_NONE);
	ft_lstclear(&tested, (void (*)(void *)) free_token);
	ft_lstclear(&excepted, (void (*)(void *)) free_token);
	return (MUNIT_OK);
}