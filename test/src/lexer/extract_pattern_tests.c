/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_pattern_tests.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 23:33:25 by apierret          #+#    #+#             */
/*   Updated: 2025/05/13 23:40:30 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "lexer.h"
#include "test.h"
#include "test_utils.h"

static t_test_case extract_pattern_cases[] =
{
	{
		.name = "no_valid_pattern",
		.input_pattern = "blabliblou",
		.expected_pattern = NULL
	},
	{
		.name = "any_pattern",
		.input_pattern = "*",
		.expected_pattern = &(t_pattern)
		{
			.prefix = NULL,
			.infixes = NULL,
			.suffix = NULL
		}
	},
	{
		.name = "prefix_file_pattern",
		.input_pattern = "*.txt",
		.expected_pattern = &(t_pattern)
		{
			.prefix = NULL,
			.infixes = NULL,
			.suffix = ".txt"
		}
	},
	{
		.name = "suffix_file_pattern",
		.input_pattern = ".*",
		.expected_pattern = &(t_pattern)
		{
			.prefix = ".",
			.infixes = NULL,
			.suffix = NULL
		}
	},
	{
		.name = "infix_file_pattern",
		.input_pattern = "*bla*",
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
	},
	{
		.name = "infixes_file_pattern",
		.input_pattern = "*bla*bli*",
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
	},
	{
		.name = "prefix_suffix_pattern",
		.input_pattern = "log*.txt",
		.expected_pattern = &(t_pattern)
		{
			.prefix = "log",
			.infixes = NULL,
			.suffix = ".txt"
		}
	},
	{
		.name = "prefix_infix_pattern",
		.input_pattern = "foo*bar*",
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
	},
	{
		.name = "infix_suffix_pattern",
		.input_pattern = "*foo*bar",
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
	},
	{
		.name = "prefix_infix_suffix_pattern",
		.input_pattern = "the*foo*bar",
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
	},
	{
		.name = "prefix_infixes_suffix_pattern",
		.input_pattern = "foo*bla*bli*blou*bar",
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
	},
	{
		.name = "empty_pattern",
		.input_pattern = "",
		.expected_pattern = NULL
	},
	{
		.name = "consecutive_stars",
		.input_pattern = "*****",
		.expected_pattern = &(t_pattern)
		{
			.prefix = NULL,
			.infixes = NULL,
			.suffix = NULL
		}
	},
	{
		.name = "prefix_consecutive_stars",
		.input_pattern = "bla*****",
		.expected_pattern = &(t_pattern)
		{
			.prefix = "bla",
			.infixes = NULL,
			.suffix = NULL
		}
	},
	{
		.name = "infix_consecutive_stars",
		.input_pattern = "**bli**",
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
	},
	{
		.name = "infixes_consecutive_stars",
		.input_pattern = "**bla**bli**blou**",
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
	},
	{
		.name = "suffix_consecutive_stars",
		.input_pattern = "*****blou",
		.expected_pattern = &(t_pattern)
		{
			.prefix = NULL,
			.infixes = NULL,
			.suffix = "blou"
		}
	},
	{
		.name = "trapped_pattern",
		.input_pattern = "bla\"*\"bli\"*\"blou",
		.expected_pattern = NULL
	},
	{
		.name = "trapped_prefix",
		.input_pattern = "\"\"*",
		.expected_pattern = &(t_pattern)
		{
			.prefix = NULL,
			.infixes = NULL,
			.suffix = NULL
		}
	},
	{
		.name = "trapped_infix",
		.input_pattern = "*\"\"*",
		.expected_pattern = &(t_pattern)
		{
			.prefix = NULL,
			.infixes = NULL,
			.suffix = NULL
		}
	},
	{
		.name = "trapped_suffix",
		.input_pattern = "*\"\"",
		.expected_pattern = &(t_pattern)
		{
			.prefix = NULL,
			.infixes = NULL,
			.suffix = NULL
		}
	},
	{ NULL }
};

static void	extract_pattern_basic_tests(void **case_name)
{
	t_test_case	*tc;
	t_pattern	*tested;
	t_error		error;
	int			equal;

	if (case_name == NULL)
		return (printf("Implementation error.\n"), assert_true(0));
	tested = NULL;
	tc = find_case(extract_pattern_cases, *case_name);
	if (tc == NULL)
		return (printf(CASE_NOT_FOUND_MSG, (char *) *case_name), assert_true(0));
	error = extract_pattern(&tested, tc->input_pattern);
	equal = pattern_equal(tested, tc->expected_pattern);
	free_pattern(tested);
	if (!equal || error != ERR_NONE)
		return(printf(FAIL_MSG, (char *) *case_name), assert_true(0));
	return (printf(SUCCESS_MSG, (char *) *case_name), assert_true(1));
}

t_test_result	extract_pattern_tests(void)
{
	const struct CMUnitTest test_cases[] = {
		cmocka_unit_test_prestate(extract_pattern_basic_tests, extract_pattern_cases[0].name),
		cmocka_unit_test_prestate(extract_pattern_basic_tests, extract_pattern_cases[1].name),
		cmocka_unit_test_prestate(extract_pattern_basic_tests, extract_pattern_cases[2].name),
		cmocka_unit_test_prestate(extract_pattern_basic_tests, extract_pattern_cases[3].name),
		cmocka_unit_test_prestate(extract_pattern_basic_tests, extract_pattern_cases[4].name),
		cmocka_unit_test_prestate(extract_pattern_basic_tests, extract_pattern_cases[5].name),
		cmocka_unit_test_prestate(extract_pattern_basic_tests, extract_pattern_cases[6].name),
		cmocka_unit_test_prestate(extract_pattern_basic_tests, extract_pattern_cases[7].name),
		cmocka_unit_test_prestate(extract_pattern_basic_tests, extract_pattern_cases[8].name),
		cmocka_unit_test_prestate(extract_pattern_basic_tests, extract_pattern_cases[9].name),
		cmocka_unit_test_prestate(extract_pattern_basic_tests, extract_pattern_cases[10].name),
		cmocka_unit_test_prestate(extract_pattern_basic_tests, extract_pattern_cases[11].name),
		cmocka_unit_test_prestate(extract_pattern_basic_tests, extract_pattern_cases[12].name),
		cmocka_unit_test_prestate(extract_pattern_basic_tests, extract_pattern_cases[13].name),
		cmocka_unit_test_prestate(extract_pattern_basic_tests, extract_pattern_cases[14].name),
		cmocka_unit_test_prestate(extract_pattern_basic_tests, extract_pattern_cases[15].name),
		cmocka_unit_test_prestate(extract_pattern_basic_tests, extract_pattern_cases[16].name),
		cmocka_unit_test_prestate(extract_pattern_basic_tests, extract_pattern_cases[17].name),
		cmocka_unit_test_prestate(extract_pattern_basic_tests, extract_pattern_cases[18].name),
		cmocka_unit_test_prestate(extract_pattern_basic_tests, extract_pattern_cases[19].name),
		cmocka_unit_test_prestate(extract_pattern_basic_tests, extract_pattern_cases[20].name),
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