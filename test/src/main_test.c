/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:22:45 by apierret          #+#    #+#             */
/*   Updated: 2025/05/06 22:20:34 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"

static MunitParameterEnum tokenize_basic_params[] = {
	{ "case", (char *[]) { "single_command", "one_arg", "multiple_args", "redir_out", "redir_append", "redir_in", "redir_heredoc", "redir_out_inverted", "redir_append_inverted", "redir_in_inverted", "redir_heredoc_inverted", "single_pipe", "multiple_pipes", "pipes_and_redir", "quoted_arg", "mixed_quotes", "complex_quotes_redir", "command_and", "command_or", "subshell", "and_with_redir", "or_with_pipe", "subshell_with_pipe", "subshell_and_redir", "nested_logic", "deep_nested_group", "multi_op_complex", NULL } },
	{ NULL, NULL }
};

static MunitParameterEnum globbing_basic_params[] = {
	{"case", (char *[]) {  "no_file_no_pattern", "file_no_pattern", "mult_files_no_pattern", "no_file_prefix", "file_prefix", "file_full_prefix", "mult_files_prefix", "file_prefix_bad", "mult_files_prefix_bad", "no_file_suffix", "file_suffix", "file_full_suffix", "mult_files_suffix", "file_suffix_bad", "mult_files_suffix_bad", "no_file_prefix_suffix", "file_prefix_suffix", "file_full_prefix_suffix", "mult_files_prefix_suffix", "file_prefix_suffix_bad", "mult_files_prefix_suffix_bad", "no_file_infixe", "file_infixe",  "file_full_infixe", "mult_files_infixe", "file_infixe_bad",  "mult_files_infixe_bad", NULL } },
	{ NULL, NULL }
};

static MunitParameterEnum parse_ast_basic_params[] = {
	{ "case", (char *[]) { "single_command", "single_command_with_path", "one_arg", "multiple_args", "redir_out", "redir_append", "redir_in", "redir_heredoc", "redir_out_inverted", "redir_append_inverted", "redir_in_inverted", "redir_heredoc_inverted", "mult_redir_out", "mult_redir_in", "mult_redir_out_both_way", "mult_redir_in_both_way", "mult_redirs_both_way", "single_pipe", "multiple_pipes", "pipes_and_redir", "command_and", "command_or", "subshell", "and_with_redir", "or_with_pipe", "subshell_with_pipe", "subshell_and_redir", "nested_logic", "nested_logic_two", "deep_nested_group", "multi_op_complex", "redir_out_alone", "redir_in_alone", NULL } },
	{ NULL, NULL }
};

static MunitParameterEnum parse_ast_error_params[] = {
	{"case", (char *[]) {"pipe_start", "pipe_end", "double_pipe", "logic_start", "logic_end", "logic_double_and", "logic_double_or", "logic_mixed_pipe", "pipe_between_ands", "redir_without_target", "redir_chain", "redir_pipe_combo", "paren_empty", "open_paren_alone", "close_paren_alone", "unbalanced_parens", "nested_unbalanced_parens", "bad_paren_combo", "mixed_logic_op", "consecutive_operators", "pipe_and_or_mix", "and_or_chain", NULL}},
	{NULL, NULL}
};

static MunitTest tests[] = {
	{"/lexer/tokenize/basic_tests", tokenize_basic_tests, NULL, NULL, MUNIT_TEST_OPTION_NONE, tokenize_basic_params },
	{"/lexer/globbing/basic_tests", globbing_basic_tests, NULL, NULL, MUNIT_TEST_OPTION_NONE, globbing_basic_params },
	{"/parser/parse_ast/basic_tests", parse_ast_basic_tests, NULL, NULL, MUNIT_TEST_OPTION_NONE, parse_ast_basic_params },
	{"/parser/parse_ast/error_tests", parse_ast_error_tests, NULL, NULL, MUNIT_TEST_OPTION_NONE, parse_ast_error_params },
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
