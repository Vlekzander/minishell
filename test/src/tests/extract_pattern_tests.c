/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_pattern_tests.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:12:31 by apierret          #+#    #+#             */
/*   Updated: 2025/05/13 11:28:56 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "tests.h"
#include "test_utils.h"

static t_case extract_pattern_cases[] =
{
	{
		.name = "no_valid_pattern",
		.input_pattern = "blabliblou",
		.excepted_pattern = NULL
	},
	{
		.name = "any_pattern",
		.input_pattern = "*",
		.excepted_pattern = &(t_pattern)
		{
			.prefix = NULL,
			.infixes = NULL,
			.suffix = NULL
		}
	},
	{
		.name = "prefix_file_pattern",
		.input_pattern = "*.txt",
		.excepted_pattern = &(t_pattern)
		{
			.prefix = NULL,
			.infixes = NULL,
			.suffix = ".txt"
		}
	},
	{
		.name = "suffix_file_pattern",
		.input_pattern = ".*",
		.excepted_pattern = &(t_pattern)
		{
			.prefix = ".",
			.infixes = NULL,
			.suffix = NULL
		}
	},
	{
		.name = "infix_file_pattern",
		.input_pattern = "*bla*",
		.excepted_pattern = &(t_pattern)
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
		.excepted_pattern = &(t_pattern)
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
		.excepted_pattern = &(t_pattern)
		{
			.prefix = "log",
			.infixes = NULL,
			.suffix = ".txt"
		}
	},
	{
		.name = "prefix_infix_pattern",
		.input_pattern = "foo*bar*",
		.excepted_pattern = &(t_pattern)
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
		.excepted_pattern = &(t_pattern)
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
		.excepted_pattern = &(t_pattern)
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
		.excepted_pattern = &(t_pattern)
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
		.excepted_pattern = NULL
	},
	{
		.name = "consecutive_stars",
		.input_pattern = "*****",
		.excepted_pattern = &(t_pattern)
		{
			.prefix = NULL,
			.infixes = NULL,
			.suffix = NULL
		}
	},
	{
		.name = "prefix_consecutive_stars",
		.input_pattern = "bla*****",
		.excepted_pattern = &(t_pattern)
		{
			.prefix = "bla",
			.infixes = NULL,
			.suffix = NULL
		}
	},
	{
		.name = "infix_consecutive_stars",
		.input_pattern = "**bli**",
		.excepted_pattern = &(t_pattern)
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
		.excepted_pattern = &(t_pattern)
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
		.excepted_pattern = &(t_pattern)
		{
			.prefix = NULL,
			.infixes = NULL,
			.suffix = "blou"
		}
	},
	{
		.name = "trapped_pattern",
		.input_pattern = "bla\"*\"bli\"*\"blou",
		.excepted_pattern = NULL
	},
	{
		.name = "trapped_prefix",
		.input_pattern = "\"\"*",
		.excepted_pattern = &(t_pattern)
		{
			.prefix = NULL,
			.infixes = NULL,
			.suffix = NULL
		}
	},
	{
		.name = "trapped_infix",
		.input_pattern = "*\"\"*",
		.excepted_pattern = &(t_pattern)
		{
			.prefix = NULL,
			.infixes = NULL,
			.suffix = NULL
		}
	},
	{
		.name = "trapped_suffix",
		.input_pattern = "*\"\"",
		.excepted_pattern = &(t_pattern)
		{
			.prefix = NULL,
			.infixes = NULL,
			.suffix = NULL
		}
	},
	{ NULL }
};

MunitResult	extract_pattern_basic_tests(const MunitParameter params[], void* data)
{
	t_case			*tc;
	t_pattern	*tested;
	t_error			error;
	int				equal;

	(void) data;
	tested = NULL;
	tc = find_case(extract_pattern_cases, munit_parameters_get(params, "case"));
	if (tc == NULL)
		return (munit_log(MUNIT_LOG_ERROR, "Test case not found"), MUNIT_ERROR);
	error = extract_pattern(&tested, tc->input_pattern);
	equal = glob_pattern_equal(tested, tc->excepted_pattern);
	free_pattern(tested);
	if (!equal || error != ERR_NONE)
		return (MUNIT_FAIL);
	return (MUNIT_OK);
}