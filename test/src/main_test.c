/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:22:45 by apierret          #+#    #+#             */
/*   Updated: 2025/04/03 13:43:38 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"

static MunitParameterEnum tokenize_basic_params[] = {
	{ "case", (char*[]) { "echo_simple", "single_command", "command_with_flag", "redir_output", "redir_input", "append_redir", "heredoc", "pipe_simple", "pipe_chain", "quoted_arg", "mixed_quotes", "command_and", "command_or", "subshell", NULL } },
	{ NULL, NULL }
};

static MunitParameterEnum parse_ast_basic_params[] = {
	{ "case", (char*[]) { "simple_echo", "pipe_ls_grep", "redir_output", NULL } },
	{ NULL, NULL }
};

static MunitTest tests[] = {
	{"/lexer/tokenize/basic_tests", tokenize_basic_tests, NULL, NULL, MUNIT_TEST_OPTION_NONE, tokenize_basic_params },
	{"/parser/parse_ast/basic_tests", parse_ast_basic_tests, NULL, NULL, MUNIT_TEST_OPTION_NONE, parse_ast_basic_params },
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
