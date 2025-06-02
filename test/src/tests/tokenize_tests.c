/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_tests.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:37:47 by apierret          #+#    #+#             */
/*   Updated: 2025/06/02 22:33:46 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "lexer.h"
#include "test.h"
#include "test_utils.h"

static t_test_case token_cases[] = {
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
	{ "quoted_arg", { { "echo \"hello world\"", { "echo", "hello world", NULL } } } },
	{ "mixed_quotes", { { "echo \"it's fine\"", { "echo", "it's fine", NULL } } } },
	{ "complex_quotes_redir", { { "ech\"o\" Hello > ne\"w\".txt\"\"", { "echo", "Hello", ">", "new.txt", NULL } } } },
	{ "command_and", { { "make && ./program", { "make", "&&", "./program", NULL } } } },
	{ "command_or", { { "./build || echo fail", { "./build", "||", "echo", "fail", NULL } } } },
	{ "subshell", { { "(pwd)", { "(", "pwd", ")", NULL } } } },
	{ "and_with_redir", { { "echo success > out.txt && cat < in.txt", { "echo", "success", ">", "out.txt", "&&", "cat", "<", "in.txt", NULL } } } },
	{ "or_with_pipe", { { "echo hello | grep world || ls | grep .h", { "echo", "hello", "|", "grep", "world", "||", "ls", "|", "grep", ".h", NULL } } } },
	{ "subshell_with_pipe", { { "(ls -la) | wc -l", { "(", "ls", "-la", ")", "|", "wc", "-l", NULL } } } },
	{ "subshell_and_redir", { { "(echo hello) > out.txt && echo done", { "(", "echo", "hello", ")", ">", "out.txt", "&&", "echo", "done", NULL } } } },
	{ "nested_logic", { { "make && (./run || echo fallback)", { "make", "&&", "(", "./run", "||", "echo", "fallback", ")", NULL } } } },
	{ "deep_nested_group", { { "(((echo ok)))", { "(", "(", "(", "echo", "ok", ")", ")", ")", NULL } } } },
	{ "multi_op_complex", { { "(cat file | grep error) && echo \"found\" || echo \"none\"", { "(", "cat", "file", "|", "grep", "error", ")", "&&", "echo", "found", "||", "echo", "none", NULL } } } },
	{ NULL }
};

static char *env[] = {
	NULL
};

static void	tokenize_basic_tests(void **case_name)
{
	t_test_case	*tc;
	t_list		*expected;
	t_list		*tested;
	int			equal;
	t_error		error;

	if (case_name == NULL)
		return (printf("Implementation error.\n"), assert_true(0));
	expected = NULL;
	tested = NULL;
	tc = find_case(token_cases, *case_name);
	if (tc == NULL)
		return (printf(CASE_NOT_FOUND_MSG, (char *) *case_name), assert_true(0));
	expected = create_token_list(tc->expected_tokens_tokenize);
	error = tokenize(&tested, env, tc->input_tokenize);
	equal = lst_equal(expected, tested, (void *) token_equal);
	ft_lstclear(&tested, (void *) free_token);
	ft_lstclear(&expected, (void *) free_token);
	if (!equal || error != ERR_NONE)
		return(printf(FAIL_MSG, (char *) *case_name), assert_true(0));
	return (printf(SUCCESS_MSG, (char *) *case_name), assert_true(1));
}

t_test_result	execute_tests(void)
{
	const struct CMUnitTest test_cases[] = {
		cmocka_unit_test_prestate(tokenize_basic_tests, token_cases[0].name),
		cmocka_unit_test_prestate(tokenize_basic_tests, token_cases[1].name),
		cmocka_unit_test_prestate(tokenize_basic_tests, token_cases[2].name),
		cmocka_unit_test_prestate(tokenize_basic_tests, token_cases[3].name),
		cmocka_unit_test_prestate(tokenize_basic_tests, token_cases[4].name),
		cmocka_unit_test_prestate(tokenize_basic_tests, token_cases[5].name),
		cmocka_unit_test_prestate(tokenize_basic_tests, token_cases[6].name),
		cmocka_unit_test_prestate(tokenize_basic_tests, token_cases[7].name),
		cmocka_unit_test_prestate(tokenize_basic_tests, token_cases[8].name),
		cmocka_unit_test_prestate(tokenize_basic_tests, token_cases[9].name),
		cmocka_unit_test_prestate(tokenize_basic_tests, token_cases[10].name),
		cmocka_unit_test_prestate(tokenize_basic_tests, token_cases[11].name),
		cmocka_unit_test_prestate(tokenize_basic_tests, token_cases[12].name),
		cmocka_unit_test_prestate(tokenize_basic_tests, token_cases[13].name),
		cmocka_unit_test_prestate(tokenize_basic_tests, token_cases[14].name),
		cmocka_unit_test_prestate(tokenize_basic_tests, token_cases[15].name),
		cmocka_unit_test_prestate(tokenize_basic_tests, token_cases[16].name),
		cmocka_unit_test_prestate(tokenize_basic_tests, token_cases[17].name),
		cmocka_unit_test_prestate(tokenize_basic_tests, token_cases[18].name),
		cmocka_unit_test_prestate(tokenize_basic_tests, token_cases[19].name),
		cmocka_unit_test_prestate(tokenize_basic_tests, token_cases[20].name),
		cmocka_unit_test_prestate(tokenize_basic_tests, token_cases[21].name),
		cmocka_unit_test_prestate(tokenize_basic_tests, token_cases[22].name),
		cmocka_unit_test_prestate(tokenize_basic_tests, token_cases[23].name),
		cmocka_unit_test_prestate(tokenize_basic_tests, token_cases[24].name),
		cmocka_unit_test_prestate(tokenize_basic_tests, token_cases[25].name),
		cmocka_unit_test_prestate(tokenize_basic_tests, token_cases[26].name),
	};
	char	name[] = "lexer/tokenize";
	t_test_result	result;

	printf(GROUP_HEADER, name);
	result.total = sizeof(test_cases)/sizeof(struct CMUnitTest);
	result.failed = cmocka_run_group_tests_name(name, test_cases, NULL, NULL);
	result.successful = result.total - result.failed;
	printf(GROUP_RESULT, name, result.successful, result.total);
	return (result);
}
