/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_var_tests.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 17:30:41 by apierret          #+#    #+#             */
/*   Updated: 2025/06/07 18:12:56 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "expand.h"
#include "test.h"
#include "test_utils.h"

static t_test_case extract_var_cases[] =
{
	{
		"simple_var",
		&(t_extract_var_case)
		{
			.input = "$VAR",
			.expected_vref = &(t_vref) {
				.str = "$VAR",
				.index = 0
			}
		}
	},
	{
		"var_prefix",
		&(t_extract_var_case)
		{
			.input = "sometext$VAR",
			.expected_vref = &(t_vref) {
				.str = "$VAR",
				.index = 8
			}
		}
	},
	{
		"var_suffix",
		&(t_extract_var_case)
		{
			.input = "$VAR sometext",
			.expected_vref = &(t_vref) {
				.str = "$VAR",
				.index = 0
			}
		}
	},
	{
		"var_prefix_suffix",
		&(t_extract_var_case)
		{
			.input = "sometext$VAR sometext",
			.expected_vref = &(t_vref) {
				.str = "$VAR",
				.index = 8
			}
		}
	},
	{
		"var_dquote",
		&(t_extract_var_case)
		{
			.input = "\"$VAR\"",
			.expected_vref = &(t_vref) {
				.str = "$VAR",
				.index = 1
			}
		}
	},
	{
		"var_squote",
		&(t_extract_var_case)
		{
			.input = "'$VAR'",
			.expected_vref = &(t_vref) {
				.str = NULL,
				.index = 0
			}
		}
	},
	{
		"digit_at_first_char",
		&(t_extract_var_case)
		{
			.input = "$4four",
			.expected_vref = &(t_vref) {
				.str = NULL,
				.index = 0
			}
		}
	},
	{
		"forbidden_char_at_end",
		&(t_extract_var_case)
		{
			.input = "$VAR.",
			.expected_vref = &(t_vref) {
				.str = "$VAR",
				.index = 0
			}
		}
	},
	{
		"last_return_value",
		&(t_extract_var_case)
		{
			.input = "$?",
			.expected_vref = &(t_vref) {
				.str = "$?",
				.index = 0
			}
		}
	},
	{
		"name_with_digit",
		&(t_extract_var_case)
		{
			.input = "$VAR1",
			.expected_vref = &(t_vref) {
				.str = "$VAR1",
				.index = 0
			}
		}
	},
	{
		"underscore_start",
		&(t_extract_var_case)
		{
			.input = "$_VAR",
			.expected_vref = &(t_vref) {
				.str = "$_VAR",
				.index = 0
			}
		}
	},
	{
		"simple_text",
		&(t_extract_var_case)
		{
			.input = "hello world",
			.expected_vref = &(t_vref) {
				.str = NULL,
				.index = 0
			}
		}
	},
	{
		"only_dollar",
		&(t_extract_var_case)
		{
			.input = "$",
			.expected_vref = &(t_vref) {
				.str = NULL,
				.index = 0
			}
		}
	},
	{
		"var_followed_by_quotes",
		&(t_extract_var_case)
		{
			.input = "$VAR\"text\"",
			.expected_vref = &(t_vref) {
				.str = "$VAR",
				.index = 0
			}
		}
	},
	{ NULL }
};

static t_vref	*expected = NULL;
static t_vref	*tested = NULL;

static int	test_setup(void **state) {
	(void) state;
	expected = NULL;
	tested = NULL;
	return (0);
}

static int	test_teardown(void **state) {
	(void) state;
	free_vref(tested);
	return (0);
}

static void	extract_var_basic_tests(void **case_name)
{
	t_test_case			*tc;
	t_extract_var_case	*data;
	t_error				error;
	int					equal;

	if (case_name == NULL)
		return(printf("Implementation error.\n"), assert_true(0));
	tc = find_case(extract_var_cases, *case_name);
	if (tc == NULL)
		return (printf(CASE_NOT_FOUND_MSG, (char *) *case_name), assert_true(0));
	data = tc->data;
	expected = data->expected_vref;
	error = extract_var(&tested, data->input, 0);
	equal = (tested->str == NULL && expected->str == NULL) || (str_equal(tested->str, expected->str) && tested->index == expected->index);
	if (error != ERR_NONE)
		return(printf(FAIL_MSG, (char *) *case_name, "error"), assert_true(0));
	if (!equal)
		return(printf(FAIL_MSG, (char *) *case_name, "not equal"), assert_true(0));
	return (printf(SUCCESS_MSG, (char *) *case_name), assert_true(1));
}

t_test_result	execute_tests(void)
{
	const struct CMUnitTest test_cases[] = {
		cmocka_unit_test_prestate_setup_teardown(extract_var_basic_tests, test_setup, test_teardown, extract_var_cases[0].name),
		cmocka_unit_test_prestate_setup_teardown(extract_var_basic_tests, test_setup, test_teardown, extract_var_cases[1].name),
		cmocka_unit_test_prestate_setup_teardown(extract_var_basic_tests, test_setup, test_teardown, extract_var_cases[2].name),
		cmocka_unit_test_prestate_setup_teardown(extract_var_basic_tests, test_setup, test_teardown, extract_var_cases[3].name),
		cmocka_unit_test_prestate_setup_teardown(extract_var_basic_tests, test_setup, test_teardown, extract_var_cases[4].name),
		cmocka_unit_test_prestate_setup_teardown(extract_var_basic_tests, test_setup, test_teardown, extract_var_cases[5].name),
		cmocka_unit_test_prestate_setup_teardown(extract_var_basic_tests, test_setup, test_teardown, extract_var_cases[6].name),
		cmocka_unit_test_prestate_setup_teardown(extract_var_basic_tests, test_setup, test_teardown, extract_var_cases[7].name),
		cmocka_unit_test_prestate_setup_teardown(extract_var_basic_tests, test_setup, test_teardown, extract_var_cases[8].name),
		cmocka_unit_test_prestate_setup_teardown(extract_var_basic_tests, test_setup, test_teardown, extract_var_cases[9].name),
		cmocka_unit_test_prestate_setup_teardown(extract_var_basic_tests, test_setup, test_teardown, extract_var_cases[10].name),
		cmocka_unit_test_prestate_setup_teardown(extract_var_basic_tests, test_setup, test_teardown, extract_var_cases[11].name),
		cmocka_unit_test_prestate_setup_teardown(extract_var_basic_tests, test_setup, test_teardown, extract_var_cases[12].name),
		cmocka_unit_test_prestate_setup_teardown(extract_var_basic_tests, test_setup, test_teardown, extract_var_cases[13].name),
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