/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_var_tests.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 12:46:11 by apierret          #+#    #+#             */
/*   Updated: 2025/06/01 20:20:29 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "lexer_internal.h"
#include "test.h"
#include "test_utils.h"

static t_test_case extract_var_cases[] =
{
	{
		.name = "var",
		.input_extract = "$VAR",
		.expected_var = &(t_vref) {
			.str = "$VAR",
			.index = 0
		}
	},
	{
		.name = "var_prefix",
		.input_extract = "sometext$VAR",
		.expected_var = &(t_vref) {
			.str = "$VAR",
			.index = 8
		}
	},
	{
		.name = "var_suffix",
		.input_extract = "$VAR sometext",
		.expected_var = &(t_vref) {
			.str = "$VAR",
			.index = 0
		}
	},
	{
		.name = "var_prefix_suffix",
		.input_extract = "sometext$VAR sometext",
		.expected_var = &(t_vref) {
			.str = "$VAR",
			.index = 8
		}
	},
	{
		.name = "var_dquote",
		.input_extract = "\"$VAR\"",
		.expected_var = &(t_vref) {
			.str = "$VAR",
			.index = 1
		}
	},
	{
		.name = "var_squote",
		.input_extract = "'$VAR'",
		.expected_var = &(t_vref) {
			.str = NULL,
			.index = 0
		}
	},
	{
		.name = "digit_at_first_char",
		.input_extract = "$4four",
		.expected_var = &(t_vref) {
			.str = NULL,
			.index = 0
		}
	},
	{
		.name = "forbidden_char_at_end",
		.input_extract = "$VAR.",
		.expected_var = &(t_vref) {
			.str = "$VAR",
			.index = 0
		}
	},
	{
		.name = "last_return_value",
		.input_extract = "$?",
		.expected_var = &(t_vref) {
			.str = "$?",
			.index = 0
		}
	},
	{
		.name = "name_with_digit",
		.input_extract = "$VAR1",
		.expected_var = &(t_vref) {
			.str = "$VAR1",
			.index = 0
		}
	},
	{
		.name = "underscore_start",
		.input_extract = "$_VAR",
		.expected_var = &(t_vref) {
			.str = "$_VAR",
			.index = 0
		}
	},
	{
		.name = "simple_text",
		.input_extract = "hello world",
		.expected_var = &(t_vref) {
			.str = NULL,
			.index = 0
		}
	},
	{
		.name = "only_dollar",
		.input_extract = "$",
		.expected_var = &(t_vref) {
			.str = NULL,
			.index = 0
		}
	},
	{
		.name = "var_followed_by_quotes",
		.input_extract = "$VAR\"text\"",
		.expected_var = &(t_vref) {
			.str = "$VAR",
			.index = 0
		}
	},
	{ NULL }
};

static void	extract_var_basic_tests(void **case_name)
{
	t_test_case	*tc;
	t_vref	*tested;
	t_vref	*base;
	t_error		error;
	int			equal;

	if (case_name == NULL)
		return (printf("Implementation error.\n"), assert_true(0));
	tc = find_case(extract_var_cases, *case_name);
	if (tc == NULL)
		return (printf(CASE_NOT_FOUND_MSG, (char *) *case_name), assert_true(0));
	base = tc->expected_var;
	error = extract_var(&tested, tc->input_extract);
	equal = (tested->str == NULL && base->str == NULL) || (str_equal(tested->str, base->str) && tested->index == base->index);
	if (!equal)
		printf("TESTED: VAR:%s - INDEX:%d\n  BASE: VAR:%s - INDEX:%d \n", tested->str, tested->index, base->str, base->index);
	free_vref(tested);
	if (!equal || error != ERR_NONE)
		return(printf(FAIL_MSG, (char *) *case_name), assert_true(0));
	return (printf(SUCCESS_MSG, (char *) *case_name), assert_true(1));
}

t_test_result	execute_tests(void)
{
	const struct CMUnitTest test_cases[] = {
		cmocka_unit_test_prestate(extract_var_basic_tests, extract_var_cases[0].name),
		cmocka_unit_test_prestate(extract_var_basic_tests, extract_var_cases[1].name),
		cmocka_unit_test_prestate(extract_var_basic_tests, extract_var_cases[2].name),
		cmocka_unit_test_prestate(extract_var_basic_tests, extract_var_cases[3].name),
		cmocka_unit_test_prestate(extract_var_basic_tests, extract_var_cases[4].name),
		cmocka_unit_test_prestate(extract_var_basic_tests, extract_var_cases[5].name),
		cmocka_unit_test_prestate(extract_var_basic_tests, extract_var_cases[6].name),
		cmocka_unit_test_prestate(extract_var_basic_tests, extract_var_cases[7].name),
		cmocka_unit_test_prestate(extract_var_basic_tests, extract_var_cases[8].name),
		cmocka_unit_test_prestate(extract_var_basic_tests, extract_var_cases[9].name),
		cmocka_unit_test_prestate(extract_var_basic_tests, extract_var_cases[10].name),
		cmocka_unit_test_prestate(extract_var_basic_tests, extract_var_cases[11].name),
		cmocka_unit_test_prestate(extract_var_basic_tests, extract_var_cases[12].name),
		cmocka_unit_test_prestate(extract_var_basic_tests, extract_var_cases[13].name),
	};
	char			name[] = "lexer/extract_var";
	t_test_result	result;

	printf(GROUP_HEADER, name);
	result.total = sizeof(test_cases)/sizeof(struct CMUnitTest);
	result.failed = cmocka_run_group_tests_name(name, test_cases, NULL, NULL);
	result.successful = result.total - result.failed;
	printf(GROUP_RESULT, name, result.successful, result.total);
	return (result);
}
