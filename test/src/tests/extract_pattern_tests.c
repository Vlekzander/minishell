/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_pattern_tests.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 17:46:17 by apierret          #+#    #+#             */
/*   Updated: 2025/06/05 18:40:28 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "lexer_internal.h"
#include "test.h"
#include "test_utils.h"

static t_test_case extract_pattern_cases[] =
{
	{
		"no_valid_pattern",
		&(t_extract_pattern_case)
		{
			.input = "blabliblou",
			.expected_pattern = NULL
		}
	},{
		"any_pattern",
		&(t_extract_pattern_case)
		{
			.input = "*",
			.expected_pattern = &(t_pattern)
			{
				.prefix = NULL,
				.infixes = NULL,
				.suffix = NULL
			}
		}
	},
	{
		"prefix_file_pattern",
		&(t_extract_pattern_case)
		{
			.input = "*.txt",
			.expected_pattern = &(t_pattern)
			{
				.prefix = NULL,
				.infixes = NULL,
				.suffix = ".txt"
			}
		}
	},
	{
		"suffix_file_pattern",
		&(t_extract_pattern_case)
		{
			.input = ".*",
			.expected_pattern = &(t_pattern)
			{
				.prefix = ".",
				.infixes = NULL,
				.suffix = NULL
			}
		}
	},
	{
		"infix_file_pattern",
		&(t_extract_pattern_case)
		{
			.input = "*bla*",
			.expected_pattern = &(t_pattern)
			{
				.prefix = NULL,
				.infixes = &(t_list)
				{
					.content = "bla",
					.next = NULL
				},
				.suffix = NULL
			}
		}
	},
	{
		"infixes_file_pattern",
		&(t_extract_pattern_case)
		{
			.input = "*bla*bli*",
			.expected_pattern = &(t_pattern)
			{
				.prefix = NULL,
				.infixes = &(t_list)
				{
					.content = "bla",
					.next = &(t_list)
					{
						.content = "bli",
						.next = NULL
					},
				},
				.suffix = NULL
			}
		}
	},
	{
		"prefix_suffix_pattern",
		&(t_extract_pattern_case)
		{
			.input = "log*.txt",
			.expected_pattern = &(t_pattern)
			{
				.prefix = "log",
				.infixes = NULL,
				.suffix = ".txt"
			}
		}
	},
	{
		"prefix_infix_pattern",
		&(t_extract_pattern_case)
		{
			.input = "foo*bar*",
			.expected_pattern = &(t_pattern)
			{
				.prefix = "foo",
				.infixes = &(t_list)
				{
					.content = "bar",
					.next = NULL
				},
				.suffix = NULL
			}
		}
	},
	{
		"infix_suffix_pattern",
		&(t_extract_pattern_case)
		{
			.input = "*foo*bar",
			.expected_pattern = &(t_pattern)
			{
				.prefix = NULL,
				.infixes = &(t_list)
				{
					.content = "foo",
					.next = NULL
				},
				.suffix = "bar"
			}
		}
	},
	{
		"prefix_infix_suffix_pattern",
		&(t_extract_pattern_case)
		{
			.input = "the*foo*bar",
			.expected_pattern = &(t_pattern)
			{
				.prefix = "the",
				.infixes = &(t_list)
				{
					.content = "foo",
					.next = NULL
				},
				.suffix = "bar"
			}
		}
	},
	{
		"prefix_infixes_suffix_pattern",
		&(t_extract_pattern_case)
		{
			.input = "foo*bla*bli*blou*bar",
			.expected_pattern = &(t_pattern)
			{
				.prefix = "foo",
				.infixes = &(t_list)
				{
					.content = "bla",
					.next = &(t_list)
					{
						.content = "bli",
						.next = &(t_list)
						{
							.content = "blou",
							.next = NULL
						},
					},
				},
				.suffix = "bar"
			}
		}
	},
	{
		"empty_pattern",
		&(t_extract_pattern_case)
		{
			.input = "",
			.expected_pattern = NULL
		}
	},
	{
		"consecutive_stars",
		&(t_extract_pattern_case)
		{
			.input = "*****",
			.expected_pattern = &(t_pattern)
			{
				.prefix = NULL,
				.infixes = NULL,
				.suffix = NULL
			}
		}
	},
	{
		"prefix_consecutive_stars",
		&(t_extract_pattern_case)
		{
			.input = "bla*****",
			.expected_pattern = &(t_pattern)
			{
				.prefix = "bla",
				.infixes = NULL,
				.suffix = NULL
			}
		}
	},
	{
		"infix_consecutive_stars",
		&(t_extract_pattern_case)
		{
			.input = "**bli**",
			.expected_pattern = &(t_pattern)
			{
				.prefix = NULL,
				.infixes = &(t_list)
				{
					.content = "bli",
					.next = NULL
				},
				.suffix = NULL
			}
		}
	},
	{
		"infixes_consecutive_stars",
		&(t_extract_pattern_case)
		{
			.input = "**bla**bli**blou**",
			.expected_pattern = &(t_pattern)
			{
				.prefix = NULL,
				.infixes = &(t_list)
				{
					.content = "bla",
					.next = &(t_list)
					{
						.content = "bli",
						.next = &(t_list)
						{
							.content = "blou",
							.next = NULL
						},
					},
				},
				.suffix = NULL
			}
		}
	},
	{
		"suffix_consecutive_stars",
		&(t_extract_pattern_case)
		{
			.input = "*****blou",
			.expected_pattern = &(t_pattern)
			{
				.prefix = NULL,
				.infixes = NULL,
				.suffix = "blou"
			}
		}
	},
	{
		"trapped_pattern",
		&(t_extract_pattern_case)
		{
			.input = "bla\"*\"bli\"*\"blou",
			.expected_pattern = NULL
		}
	},
	{
		"trapped_prefix",
		&(t_extract_pattern_case)
		{
			.input = "\"\"*",
			.expected_pattern = &(t_pattern)
			{
				.prefix = NULL,
				.infixes = NULL,
				.suffix = NULL
			}
		}
	},
	{
		"trapped_infix",
		&(t_extract_pattern_case)
		{
			.input = "*\"\"*",
			.expected_pattern = &(t_pattern)
			{
				.prefix = NULL,
				.infixes = NULL,
				.suffix = NULL
			}
		}
	},
	{
		"trapped_suffix",
		&(t_extract_pattern_case)
		{
			.input = "*\"\"",
			.expected_pattern = &(t_pattern)
			{
				.prefix = NULL,
				.infixes = NULL,
				.suffix = NULL
			}
		}
	},

	{ NULL }
};

static t_pattern	*expected = NULL;
static t_pattern	*tested = NULL;

static int test_setup(void **state) {
	(void) state;
	expected = NULL;
	tested = NULL;
	return (0);
}

static int test_teardown(void **state) {
	(void) state;
	free_pattern(tested);
	return (0);
}

static void	extract_pattern_basic_tests(void **case_name)
{
	t_test_case				*tc;
	t_extract_pattern_case	*data;
	t_error					error;
	int						equal;

	if (case_name == NULL)
		return(printf("Implementation error.\n"), assert_true(0));
	tc = find_case(extract_pattern_cases, *case_name);
	if (tc == NULL)
		return (printf(CASE_NOT_FOUND_MSG, (char *) *case_name), assert_true(0));
	data = tc->data;
	error = extract_pattern(&tested, data->input);
	equal = pattern_equal(tested, data->expected_pattern);
	if (error != ERR_NONE)
		return(printf(FAIL_MSG, (char *) *case_name, "error"), assert_true(0));
	if (!equal)
		return(printf(FAIL_MSG, (char *) *case_name, "not equal"), assert_true(0));
	return (printf(SUCCESS_MSG, (char *) *case_name), assert_true(1));
}

t_test_result	execute_tests(void)
{
	const struct CMUnitTest test_cases[] = {
		cmocka_unit_test_prestate_setup_teardown(extract_pattern_basic_tests, test_setup, test_teardown, extract_pattern_cases[0].name),
		cmocka_unit_test_prestate_setup_teardown(extract_pattern_basic_tests, test_setup, test_teardown, extract_pattern_cases[1].name),
		cmocka_unit_test_prestate_setup_teardown(extract_pattern_basic_tests, test_setup, test_teardown, extract_pattern_cases[2].name),
		cmocka_unit_test_prestate_setup_teardown(extract_pattern_basic_tests, test_setup, test_teardown, extract_pattern_cases[3].name),
		cmocka_unit_test_prestate_setup_teardown(extract_pattern_basic_tests, test_setup, test_teardown, extract_pattern_cases[4].name),
		cmocka_unit_test_prestate_setup_teardown(extract_pattern_basic_tests, test_setup, test_teardown, extract_pattern_cases[5].name),
		cmocka_unit_test_prestate_setup_teardown(extract_pattern_basic_tests, test_setup, test_teardown, extract_pattern_cases[6].name),
		cmocka_unit_test_prestate_setup_teardown(extract_pattern_basic_tests, test_setup, test_teardown, extract_pattern_cases[7].name),
		cmocka_unit_test_prestate_setup_teardown(extract_pattern_basic_tests, test_setup, test_teardown, extract_pattern_cases[8].name),
		cmocka_unit_test_prestate_setup_teardown(extract_pattern_basic_tests, test_setup, test_teardown, extract_pattern_cases[9].name),
		cmocka_unit_test_prestate_setup_teardown(extract_pattern_basic_tests, test_setup, test_teardown, extract_pattern_cases[10].name),
		cmocka_unit_test_prestate_setup_teardown(extract_pattern_basic_tests, test_setup, test_teardown, extract_pattern_cases[11].name),
		cmocka_unit_test_prestate_setup_teardown(extract_pattern_basic_tests, test_setup, test_teardown, extract_pattern_cases[12].name),
		cmocka_unit_test_prestate_setup_teardown(extract_pattern_basic_tests, test_setup, test_teardown, extract_pattern_cases[13].name),
		cmocka_unit_test_prestate_setup_teardown(extract_pattern_basic_tests, test_setup, test_teardown, extract_pattern_cases[14].name),
		cmocka_unit_test_prestate_setup_teardown(extract_pattern_basic_tests, test_setup, test_teardown, extract_pattern_cases[15].name),
		cmocka_unit_test_prestate_setup_teardown(extract_pattern_basic_tests, test_setup, test_teardown, extract_pattern_cases[16].name),
		cmocka_unit_test_prestate_setup_teardown(extract_pattern_basic_tests, test_setup, test_teardown, extract_pattern_cases[17].name),
		cmocka_unit_test_prestate_setup_teardown(extract_pattern_basic_tests, test_setup, test_teardown, extract_pattern_cases[18].name),
		cmocka_unit_test_prestate_setup_teardown(extract_pattern_basic_tests, test_setup, test_teardown, extract_pattern_cases[19].name),
		cmocka_unit_test_prestate_setup_teardown(extract_pattern_basic_tests, test_setup, test_teardown, extract_pattern_cases[20].name),
	};
	char			name[] = "lexer/extract_pattern";
	t_test_result	result;

	printf(GROUP_HEADER, name);
	result.total = sizeof(test_cases)/sizeof(struct CMUnitTest);
	result.failed = cmocka_run_group_tests_name(name, test_cases, NULL, NULL);
	result.successful = result.total - result.failed;
	printf(GROUP_RESULT, name, result.successful, result.total);
	return (result);
}
