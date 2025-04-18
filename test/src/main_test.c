/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:22:45 by apierret          #+#    #+#             */
/*   Updated: 2025/04/18 16:35:46 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"

static MunitParameterEnum tokenize_basic_params[] = {
	{ "case", (char*[]) { "single_command", "one_arg", "multiple_args", "redir_out", "redir_append", "redir_in", "redir_heredoc", "redir_out_inverted", "redir_append_inverted", "redir_in_inverted", "redir_heredoc_inverted", "single_pipe", "multiple_pipes", "pipes_and_redir", "quoted_arg", "mixed_quotes", "command_and", "command_or", "subshell", "and_with_redir", "or_with_pipe", "subshell_with_pipe", "subshell_and_redir", "nested_logic", "deep_nested_group", "multi_op_complex", NULL } },
	{ NULL, NULL }
};

static MunitParameterEnum parse_ast_basic_params[] = {
	{ "case", (char*[]) { "single_command", "single_command_with_path", "one_arg", "multiple_args", "redir_out", "redir_append", "redir_in", "redir_heredoc", "redir_out_inverted", "redir_append_inverted", "redir_in_inverted", "redir_heredoc_inverted", "single_pipe", "multiple_pipes", "pipes_and_redir", "command_and", "command_or", "subshell", "and_with_redir", "or_with_pipe", "subshell_with_pipe", "subshell_and_redir", "nested_logic", "deep_nested_group", "multi_op_complex", NULL } },
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
