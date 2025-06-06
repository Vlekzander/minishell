/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_integration_tests.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 17:52:16 by apierret          #+#    #+#             */
/*   Updated: 2025/06/06 18:17:55 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "lexer.h"
#include "test.h"
#include "test_utils.h"

static t_test_case tokenize_cases[] = {
	{ "single_command", &(t_tokenize_case) { "pwd", {"pwd", NULL} } },
	{"one_arg", &(t_tokenize_case) { "echo hello", { "echo", "hello", NULL } } },
	{ "multiple_args", &(t_tokenize_case) { "echo hello world", { "echo", "hello", "world", NULL } } },
	{ "redir_out", &(t_tokenize_case) { "echo hello > out.txt", { "echo", "hello", ">", "out.txt", NULL } } },
	{ "redir_append", &(t_tokenize_case) { "echo world >> out.txt", { "echo", "world", ">>", "out.txt", NULL } } },
	{ "redir_in", &(t_tokenize_case) { "cat < in.txt", { "cat", "<", "in.txt", NULL } } },
	{ "redir_heredoc", &(t_tokenize_case) { "cat << END", { "cat", "<<", "END", NULL } } },
	{ "redir_out_inverted", &(t_tokenize_case) { "> out.txt echo hello", { ">", "out.txt", "echo", "hello", NULL } } },
	{ "redir_append_inverted", &(t_tokenize_case) { ">> out.txt echo world", { ">>", "out.txt", "echo", "world", NULL } } },
	{ "redir_in_inverted", &(t_tokenize_case) { "< in.txt cat", { "<", "in.txt", "cat", NULL } } },
	{ "redir_heredoc_inverted", &(t_tokenize_case) { "<< END cat", { "<<", "END", "cat", NULL } } },
	{ "single_pipe", &(t_tokenize_case) { "ls | grep .c", { "ls", "|", "grep", ".c", NULL } } },
	{ "multiple_pipes", &(t_tokenize_case) { "ls | grep .c | wc -l", { "ls", "|", "grep", ".c", "|", "wc", "-l", NULL } } },
	{ "pipes_and_redir", &(t_tokenize_case) { "cat < in.txt | grep hello | wc -l > out.txt", { "cat", "<", "in.txt", "|", "grep", "hello", "|", "wc", "-l", ">", "out.txt", NULL } } },
	{ "quoted_arg", &(t_tokenize_case) { "echo \"hello world\"", { "echo", "hello world", NULL } } },
	{ "mixed_quotes", &(t_tokenize_case) { "echo \"it's fine\"", { "echo", "it's fine", NULL } } },
	{ "complex_quotes_redir", &(t_tokenize_case) { "ech\"o\" Hello > ne\"w\".txt\"\"", { "echo", "Hello", ">", "new.txt", NULL } } },
	{ "command_and", &(t_tokenize_case) { "make && ./program", { "make", "&&", "./program", NULL } } },
	{ "command_or", &(t_tokenize_case) { "./build || echo fail", { "./build", "||", "echo", "fail", NULL } } },
	{ "subshell", &(t_tokenize_case) { "(pwd)", { "(", "pwd", ")", NULL } } },
	{ "and_with_redir", &(t_tokenize_case) { "echo success > out.txt && cat < in.txt", { "echo", "success", ">", "out.txt", "&&", "cat", "<", "in.txt", NULL } } },
	{ "or_with_pipe", &(t_tokenize_case) { "echo hello | grep world || ls | grep .h", { "echo", "hello", "|", "grep", "world", "||", "ls", "|", "grep", ".h", NULL } } },
	{ "subshell_with_pipe", &(t_tokenize_case) { "(ls -la) | wc -l", { "(", "ls", "-la", ")", "|", "wc", "-l", NULL } } },
	{ "subshell_and_redir", &(t_tokenize_case) { "(echo hello) > out.txt && echo done", { "(", "echo", "hello", ")", ">", "out.txt", "&&", "echo", "done", NULL } } },
	{ "nested_logic", &(t_tokenize_case) { "make && (./run || echo fallback)", { "make", "&&", "(", "./run", "||", "echo", "fallback", ")", NULL } } },
	{ "deep_nested_group", &(t_tokenize_case) { "(((echo ok)))", { "(", "(", "(", "echo", "ok", ")", ")", ")", NULL } } },
	{ "multi_op_complex", &(t_tokenize_case) { "(cat file | grep error) && echo \"found\" || echo \"none\"", { "(", "cat", "file", "|", "grep", "error", ")", "&&", "echo", "found", "||", "echo", "none", NULL } } },
	{ "expand_one_word_var", &(t_tokenize_case) { "echo hello $USER", {"echo", "hello", "alex", NULL} } },
	{ "expand_multiples_word_var", &(t_tokenize_case) { "echo $WELCOME2", {"echo", "Hello", "World", NULL} } },
	{ "expand_multiples_vars", &(t_tokenize_case) { "echo $USER $WELCOME2", {"echo", "alex", "Hello", "World", NULL} } },
	{ "expand_var_dquoted", &(t_tokenize_case) { "echo hello \"$USER\"", {"echo", "hello", "alex", NULL} } },
	{ "expand_var_squoted", &(t_tokenize_case) { "echo hello \'$USER\'", {"echo", "hello", "$USER", NULL} } },
	{ "expand_var_content_quoted", &(t_tokenize_case) { "echo $WELCOME3", {"echo", "Hello", "Alex", NULL} } },
	{ "expand_var_prefix", &(t_tokenize_case) { "echo user:$USER", {"echo", "user:alex", NULL} } },
	{ "expand_empty_var", &(t_tokenize_case) { "echo hello $EMPTY", {"echo", "hello", NULL} } },
	{ "expand_no_var", &(t_tokenize_case) { "echo hello $NONE", {"echo", "hello", NULL} } },
	{ "expand_var_quotted_suffix", &(t_tokenize_case) { "echo $USER\"__SUFFIX__\"", {"echo", "alex__SUFFIX__", NULL} } },
	{ "expand_empty_var_only", &(t_tokenize_case) { "$EMPTY", {NULL} } },
	{ "expand_wildcard_from_env", &(t_tokenize_case) { "echo $WILDCARD", {"echo", ".git", ".gitignore", ".idea", NULL} } },
	{ "expand_wildcard_from_env", &(t_tokenize_case) { "echo $WILDCARDS", {"echo", "BEFORE", "*.txt", "IN", "Makefile", "libft", "AFTER", "Makefile", "bin_test", "include", "libft", "src", "test", NULL} } },
	{ NULL }
};

static char *env[] = {
	"USER=alex",
	"WELCOME=Hello",
	"WELCOME2=Hello World",
	"WELCOME3=\"Hello Alex\"",
	"EMPTY=",
	"WILDCARD=.*",
	"WILDCARDS=BEFORE *.txt IN *f* AFTER *",
	"AAA=$BBB",
	"BBB=$CCC",
	"CCC=Valorant",
	NULL
};
static t_list	*expected = NULL;
static t_list	*tested = NULL;

t_error	__wrap_expand(t_list **tk_list, char **env)
{
	(void) tk_list;
	(void) env;
	return (ERR_NONE);
}

static int	test_setup(void **state)
{
	(void) state;
	expected = NULL;
	tested = NULL;
	return (0);
}

static int	test_teardown(void **state) {
	(void) state;
	ft_lstclear(&expected, (void *) free_token);
	ft_lstclear(&tested, (void *) free_token);
	return (0);
}

static void	tokenize_integration_test(void **case_name)
{
	t_test_case		*tc;
	t_tokenize_case	*data;
	t_error			error;
	int				equal;

	if (case_name == NULL)
		return(printf("Implementation error.\n"), assert_true(0));
	tc = find_case(tokenize_cases, *case_name);
	if (tc == NULL)
		return (printf(CASE_NOT_FOUND_MSG, (char *) *case_name), assert_true(0));
	data = tc->data;
	expected = create_token_list(data->expected_tokens);
	if (expected == NULL && data->expected_tokens[0] != NULL)
		return(printf(FAIL_MSG, (char *) *case_name, "create_token_list failed"), assert_true(0));
	error = tokenize(&tested, env, data->input);
	equal = lst_equal(expected, tested, (void *) token_equal);
	if (error != ERR_NONE)
		return(printf(FAIL_MSG, (char *) *case_name, "error"), assert_true(0));
	if (!equal)
		return(printf(FAIL_MSG, (char *) *case_name, "not equal"), assert_true(0));
	return (printf(SUCCESS_MSG, (char *) *case_name), assert_true(1));
}

t_test_result	execute_tests(void)
{
	const struct CMUnitTest test_cases[] = {
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[0].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[1].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[2].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[3].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[4].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[5].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[6].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[7].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[8].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[9].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[10].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[11].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[12].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[13].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[14].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[15].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[16].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[17].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[18].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[19].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[20].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[21].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[22].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[23].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[24].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[25].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[26].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[27].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[28].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[29].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[30].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[31].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[32].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[33].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[34].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[35].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[36].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[37].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[38].name),
		cmocka_unit_test_prestate_setup_teardown(tokenize_integration_test, test_setup, test_teardown, tokenize_cases[39].name),
	};
	char	name[] = "lexer/tokenize_integration";
	t_test_result	result;

	printf(GROUP_HEADER, name);
	result.total = sizeof(test_cases)/sizeof(struct CMUnitTest);
	result.failed = cmocka_run_group_tests_name(name, test_cases, NULL, NULL);
	result.successful = result.total - result.failed;
	printf(GROUP_RESULT, name, result.successful, result.total);
	return (result);
}
