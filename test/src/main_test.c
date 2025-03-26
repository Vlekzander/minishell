/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:22:45 by apierret          #+#    #+#             */
/*   Updated: 2025/03/26 12:04:50 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"
#include "lexer.h"
#include "munit.h"
#include "test_utils.h"

typedef struct {
	char* name;
	char* input;
	char* expected_tokens[10];
} TokenCase;

static TokenCase token_cases[] = {
	{ "echo_simple", "echo hello", { "echo", "hello", NULL } },
	{ "single_command", "pwd", { "pwd", NULL } },
	{ "command_with_flag", "ls -a", { "ls", "-a", NULL } },
	{ "redir_output", "echo hello > out.txt", { "echo", "hello", ">", "out.txt", NULL } },
	{ "redir_input", "cat < file.txt", { "cat", "<", "file.txt", NULL } },
	{ "append_redir", "echo hi >> log.txt", { "echo", "hi", ">>", "log.txt", NULL } },
	{ "heredoc", "cat << EOF", { "cat", "<<", "EOF", NULL } },
	{ "pipe_simple", "ls | grep .c", { "ls", "|", "grep", ".c", NULL } },
	{ "pipe_chain", "cat file | sort | uniq", { "cat", "file", "|", "sort", "|", "uniq", NULL } },
	{ "quoted_arg", "echo \"hello world\"", { "echo", "hello world", NULL } },
	{ "mixed_quotes", "echo \"it's fine\"", { "echo", "it's fine", NULL } },
	{ "command_and", "make && ./program", { "make", "&&", "./program", NULL } },
	{ "command_or", "./build || echo \"fail\"", { "./build", "||", "echo", "fail", NULL } },
	{ "subshell", "(pwd)", { "(", "pwd", ")", NULL } },
};

static MunitParameterEnum tokenize_basic_params[] = {
	{ "case", (char*[]) { "echo_simple", "single_command", "command_with_flag", "redir_output", "redir_input", "append_redir", "heredoc", "pipe_simple", "pipe_chain", "quoted_arg", "mixed_quotes", "command_and", "command_or", "subshell", NULL } },
	{ NULL, NULL }
};

static TokenCase	*find_case(const char* name)
{
	for (size_t i = 0; i < sizeof(token_cases)/sizeof(TokenCase); i++)
	{
		if (strcmp(token_cases[i].name, name) == 0)
			return (&token_cases[i]);
	}
	return (NULL);
}

static MunitResult	tokenize_basic_tests(const MunitParameter params[], void* data)
{
	(void) data;
	TokenCase	*tc = find_case(munit_parameters_get(params, "case"));
	t_list		*excepted = NULL;
	t_list		*tested = NULL;
	t_error		result;

	excepted = create_token_list(tc->expected_tokens);
	result = tokenize(&tested, tc->input);
	munit_assert_true(lst_equal(excepted, tested, token_equal));
	munit_assert_true(result == ERR_NONE);
	ft_lstclear(&tested, (void (*)(void *)) free_token);
	ft_lstclear(&excepted, (void (*)(void *)) free_token);
	return (MUNIT_OK);
}

static MunitTest tests[] = {
	{"/lexer/tokenize/basic_tests", tokenize_basic_tests, NULL, NULL, MUNIT_TEST_OPTION_NONE, tokenize_basic_params },
	{ NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite suite = {
	"",
	tests,
	NULL,
	1,
	MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv[])
{
	return (munit_suite_main(&suite, NULL, argc, argv));
}
