/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing_tests.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 22:10:14 by apierret          #+#    #+#             */
/*   Updated: 2025/05/13 23:38:45 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "lexer.h"
#include "test.h"
#include "test_utils.h"

static t_test_case globbing_cases[] =
{
	{
		.name = "no_file_no_pattern",
		.in_files = NULL,
		.patterns = &(t_pattern){
			.prefix = NULL,
			.suffix = NULL,
			.infixes = NULL
		},
		.expected_out_files = NULL
	},
	{
		.name = "file_no_pattern",
		.in_files = &(t_list) {
			.content = "file.txt",
			.next = NULL
		},
		.patterns = &(t_pattern){
			.prefix = NULL,
			.suffix = NULL,
			.infixes = NULL
		},
		.expected_out_files = &(t_list) {
			.content = "file.txt",
			.next = NULL
		},
	},
{
	.name = "mult_files_no_pattern",
		.in_files = &(t_list) {
			.content = "file1.txt",
			.next = &(t_list) {
				.content = "file2.txt",
				.next = &(t_list) {
					.content = "file3.txt",
					.next = NULL
				},
			},
		},
		.patterns = &(t_pattern){
			.prefix = NULL,
			.suffix = NULL,
			.infixes = NULL
		},
		.expected_out_files = &(t_list) {
			.content = "file1.txt",
			.next = &(t_list) {
				.content = "file2.txt",
				.next = &(t_list) {
					.content = "file3.txt",
					.next = NULL
				},
			},
		},
	},
	{
		.name = "no_file_prefix",
		.in_files = NULL,
		.patterns = &(t_pattern){
			.prefix = "file",
			.suffix = NULL,
			.infixes = NULL
		},
		.expected_out_files = NULL
	},
	{
		.name = "file_prefix",
		.in_files = &(t_list) {
			.content = "file.txt",
			.next = &(t_list) {
				.content = "another.md",
				.next = &(t_list) {
					.content = "something.jpg",
					.next = NULL
				},
			},
		},
		.patterns = &(t_pattern){
			.prefix = "file",
			.suffix = NULL,
			.infixes = NULL
		},
		.expected_out_files = &(t_list) {
			.content = "file.txt",
			.next = NULL
		},
	},
	{
		.name = "file_full_prefix",
		.in_files = &(t_list) {
			.content = "file.txt",
			.next = &(t_list) {
				.content = "another.md",
				.next = &(t_list) {
					.content = "something.jpg",
					.next = NULL
				},
			},
		},
		.patterns = &(t_pattern){
			.prefix = "file.txt",
			.suffix = NULL,
			.infixes = NULL
		},
		.expected_out_files = &(t_list) {
			.content = "file.txt",
			.next = NULL
		},
	},
	{
		.name = "mult_files_prefix",
		.in_files = &(t_list) {
			.content = "file1.txt",
			.next = &(t_list) {
				.content = "file2.txt",
				.next = &(t_list) {
					.content = "file3.txt",
					.next = &(t_list) {
						.content = "another.md",
						.next = &(t_list) {
							.content = "something.jpg",
							.next = NULL
						},
					},
				},
			},
		},
		.patterns = &(t_pattern){
			.prefix = "file",
			.suffix = NULL,
			.infixes = NULL
		},
		.expected_out_files = &(t_list) {
			.content = "file1.txt",
			.next = &(t_list) {
				.content = "file2.txt",
				.next = &(t_list) {
					.content = "file3.txt",
					.next = NULL
				},
			},
		},
	},
	{
		.name = "file_prefix_bad",
		.in_files = &(t_list) {
			.content = "file.txt",
			.next = NULL
		},
		.patterns = &(t_pattern){
			.prefix = "app",
			.suffix = NULL,
			.infixes = NULL
		},
		.expected_out_files = NULL
	},
	{
		.name = "mult_files_prefix_bad",
		.in_files = &(t_list) {
			.content = "file1.txt",
			.next = &(t_list) {
				.content = "file2.txt",
				.next = &(t_list) {
					.content = "file3.txt",
					.next = NULL
				},
			},
		},
		.patterns = &(t_pattern){
			.prefix = "app",
			.suffix = NULL,
			.infixes = NULL
		},
		.expected_out_files = NULL
	},
	{
		.name = "no_file_suffix",
		.in_files = NULL,
		.patterns = &(t_pattern){
			.prefix = NULL,
			.suffix = ".txt",
			.infixes = NULL
		},
		.expected_out_files = NULL
	},
	{
		.name = "file_suffix",
		.in_files = &(t_list) {
			.content = "file.txt",
			.next = &(t_list) {
				.content = "another.md",
				.next = &(t_list) {
					.content = "something.jpg",
					.next = NULL
				},
			},
		},
		.patterns = &(t_pattern){
			.prefix = NULL,
			.suffix = ".txt",
			.infixes = NULL
		},
		.expected_out_files = &(t_list) {
			.content = "file.txt",
			.next = NULL
		},
	},
	{
		.name = "file_full_suffix",
		.in_files = &(t_list) {
			.content = "file.txt",
			.next = &(t_list) {
				.content = "another.md",
				.next = &(t_list) {
					.content = "something.jpg",
					.next = NULL
				},
			},
		},
		.patterns = &(t_pattern){
			.prefix = NULL,
			.suffix = "file.txt",
			.infixes = NULL
		},
		.expected_out_files = &(t_list) {
			.content = "file.txt",
			.next = NULL
		},
	},
	{
		.name = "mult_files_suffix",
		.in_files = &(t_list) {
			.content = "file1.txt",
			.next = &(t_list) {
				.content = "file2.txt",
				.next = &(t_list) {
					.content = "file3.txt",
					.next = &(t_list) {
						.content = "another.md",
						.next = &(t_list) {
							.content = "something.jpg",
							.next = NULL
						},
					},
				},
			},
		},
		.patterns = &(t_pattern){
			.prefix = NULL,
			.suffix = ".txt",
			.infixes = NULL
		},
		.expected_out_files = &(t_list) {
			.content = "file1.txt",
			.next = &(t_list) {
				.content = "file2.txt",
				.next = &(t_list) {
					.content = "file3.txt",
					.next = NULL
				},
			},
		},
	},
	{
		.name = "file_suffix_bad",
		.in_files = &(t_list) {
			.content = "file.txt",
			.next = NULL
		},
		.patterns = &(t_pattern){
			.prefix = NULL,
			.suffix = ".md",
			.infixes = NULL
		},
		.expected_out_files = NULL
	},
	{
		.name = "mult_files_suffix_bad",
		.in_files = &(t_list) {
			.content = "file1.txt",
			.next = &(t_list) {
				.content = "file2.txt",
				.next = &(t_list) {
					.content = "file3.txt",
					.next = NULL
				},
			},
		},
		.patterns = &(t_pattern){
			.prefix = NULL,
			.suffix = ".md",
			.infixes = NULL
		},
		.expected_out_files = NULL
	},
	{
		.name = "no_file_prefix_suffix",
		.in_files = NULL,
		.patterns = &(t_pattern){
			.prefix = "fi",
			.suffix = ".txt",
			.infixes = NULL
		},
		.expected_out_files = NULL
	},
	{
		.name = "file_prefix_suffix",
		.in_files = &(t_list) {
			.content = "file.txt",
			.next = &(t_list) {
				.content = "another.md",
				.next = &(t_list) {
					.content = "something.jpg",
					.next = &(t_list) {
						.content = "filename.md",
						.next = NULL
					},
				},
			},
		},
		.patterns = &(t_pattern){
			.prefix = "fi",
			.suffix = ".txt",
			.infixes = NULL
		},
		.expected_out_files = &(t_list) {
			.content = "file.txt",
			.next = NULL
		},
	},
	{
		.name = "file_full_prefix_suffix",
		.in_files = &(t_list) {
			.content = "file.txt",
			.next = &(t_list) {
				.content = "another.md",
				.next = &(t_list) {
					.content = "something.jpg",
					.next = NULL
				},
			},
		},
		.patterns = &(t_pattern){
			.prefix = "file",
			.suffix = ".txt",
			.infixes = NULL
		},
		.expected_out_files = &(t_list) {
			.content = "file.txt",
			.next = NULL
		},
	},
	{
		.name = "mult_files_prefix_suffix",
		.in_files = &(t_list) {
			.content = "file1.txt",
			.next = &(t_list) {
				.content = "file2.txt",
				.next = &(t_list) {
					.content = "file3.txt",
					.next = &(t_list) {
						.content = "another.md",
						.next = &(t_list) {
							.content = "something.jpg",
							.next = NULL
						},
					},
				},
			},
		},
		.patterns = &(t_pattern){
			.prefix = "file",
			.suffix = ".txt",
			.infixes = NULL
		},
		.expected_out_files = &(t_list) {
			.content = "file1.txt",
			.next = &(t_list) {
				.content = "file2.txt",
				.next = &(t_list) {
					.content = "file3.txt",
					.next = NULL
				},
			},
		},
	},
	{
		.name = "file_prefix_suffix_bad",
		.in_files = &(t_list) {
			.content = "file.txt",
			.next = NULL
		},
		.patterns = &(t_pattern){
			.prefix = "doc",
			.suffix = ".md",
			.infixes = NULL
		},
		.expected_out_files = NULL
	},
	{
		.name = "mult_files_prefix_suffix_bad",
		.in_files = &(t_list) {
			.content = "file1.txt",
			.next = &(t_list) {
				.content = "file2.txt",
				.next = &(t_list) {
					.content = "file3.txt",
					.next = NULL
				},
			},
		},
		.patterns = &(t_pattern){
			.prefix = "doc",
			.suffix = ".md",
			.infixes = NULL
		},
		.expected_out_files = NULL
	},
	{
		.name = "no_file_infixe",
		.in_files = NULL,
		.patterns = &(t_pattern){
			.prefix = NULL,
			.suffix = NULL,
			.infixes = &(t_list)
			{
				.content = "le.tx",
				.next = NULL
			}
		},
		.expected_out_files = NULL
	},
	{
		.name = "file_infixe",
		.in_files = &(t_list) {
			.content = "file.txt",
			.next = &(t_list) {
				.content = "another.md",
				.next = &(t_list) {
					.content = "something.jpg",
					.next = NULL
				},
			},
		},
		.patterns = &(t_pattern){
			.prefix = NULL,
			.suffix = NULL,
			.infixes = &(t_list)
			{
				.content = "le.tx",
				.next = NULL
			}
		},
		.expected_out_files = &(t_list) {
			.content = "file.txt",
			.next = NULL
		},
	},
	{
		.name = "file_full_infixe",
		.in_files = &(t_list) {
			.content = "file.txt",
			.next = &(t_list) {
				.content = "another.md",
				.next = &(t_list) {
					.content = "something.jpg",
					.next = NULL
				},
			},
		},
		.patterns = &(t_pattern){
			.prefix = NULL,
			.suffix = NULL,
			.infixes = &(t_list)
			{
				.content = "file",
				.next = &(t_list)
				{
					.content = ".txt",
					.next = NULL
				}
			}
		},
		.expected_out_files = &(t_list) {
			.content = "file.txt",
			.next = NULL
		},
	},
	{
		.name = "mult_files_infixe",
		.in_files = &(t_list) {
			.content = "file1.txt",
			.next = &(t_list) {
				.content = "file2.txt",
				.next = &(t_list) {
					.content = "file3.txt",
					.next = &(t_list) {
						.content = "another.md",
						.next = &(t_list) {
							.content = "something.jpg",
							.next = NULL
						},
					},
				},
			},
		},
		.patterns = &(t_pattern){
			.prefix = NULL,
			.suffix = NULL,
			.infixes = &(t_list)
			{
				.content = "file",
				.next = &(t_list)
				{
					.content = ".txt",
					.next = NULL
				}
			}
		},
		.expected_out_files = &(t_list) {
			.content = "file1.txt",
			.next = &(t_list) {
				.content = "file2.txt",
				.next = &(t_list) {
					.content = "file3.txt",
					.next = NULL
				},
			},
		},
	},
	{
		.name = "file_infixe_bad",
		.in_files = &(t_list) {
			.content = "file.txt",
			.next = NULL
		},
		.patterns = &(t_pattern){
			.prefix = NULL,
			.suffix = NULL,
			.infixes = &(t_list)
			{
				.content = "doc",
				.next = &(t_list)
				{
					.content = ".md",
					.next = NULL
				}
			}
		},
		.expected_out_files = NULL
	},
	{
		.name = "mult_files_infixe_bad",
		.in_files = &(t_list) {
			.content = "file1.txt",
			.next = &(t_list) {
				.content = "file2.txt",
				.next = &(t_list) {
					.content = "file3.txt",
					.next = NULL
				},
			},
		},
		.patterns = &(t_pattern){
			.prefix = NULL,
			.suffix = NULL,
			.infixes = &(t_list)
			{
				.content = "doc",
				.next = &(t_list)
				{
					.content = ".md",
					.next = NULL
				}
			}
		},
		.expected_out_files = NULL
	},
	{
		.name = "any_with_hidden_files",
		.in_files = &(t_list) {
			.content = "file1.txt",
			.next = &(t_list) {
				.content = "file2.txt",
				.next = &(t_list) {
					.content = "file3.txt",
					.next = &(t_list) {
						.content = ".hidden1.txt",
						.next = &(t_list) {
							.content = ".hidden2.txt",
							.next = NULL
						},
					},
				},
			},
		},
		.patterns = &(t_pattern){
			.prefix = NULL,
			.suffix = NULL,
			.infixes = NULL
		},
		.expected_out_files = &(t_list) {
			.content = "file1.txt",
			.next = &(t_list) {
				.content = "file2.txt",
				.next = &(t_list) {
					.content = "file3.txt",
					.next = NULL
				},
			},
		},
	},
	{
		.name = "prefix_hidden_files",
		.in_files = &(t_list) {
			.content = "file1.txt",
			.next = &(t_list) {
				.content = "file2.txt",
				.next = &(t_list) {
					.content = "file3.txt",
					.next = &(t_list) {
						.content = ".hidden1.txt",
						.next = &(t_list) {
							.content = ".hidden2.txt",
							.next = NULL
						},
					},
				},
			},
		},
		.patterns = &(t_pattern){
			.prefix = ".",
			.suffix = NULL,
			.infixes = NULL
		},
		.expected_out_files = &(t_list) {
			.content = ".hidden1.txt",
			.next = &(t_list) {
				.content = ".hidden2.txt",
				.next = NULL
			},
		},
	},
	{
		.name = "prefix_not_prefix",
		.in_files = &(t_list) {
			.content = "file1.txt",
			.next = &(t_list) {
				.content = "file2.txt",
				.next = &(t_list) {
					.content = "file3.txt",
					.next = NULL
				},
			},
		},
		.patterns = &(t_pattern){
			.prefix = "txt",
			.suffix = NULL,
			.infixes = NULL
		},
		.expected_out_files = NULL
	},
	{
		.name = "suffix_not_suffix",
		.in_files = &(t_list) {
			.content = "file1.txtfile",
			.next = &(t_list) {
				.content = "file2.txtfile",
				.next = &(t_list) {
					.content = "file3.txtfile",
					.next = NULL
				},
			},
		},
		.patterns = &(t_pattern){
			.prefix = NULL,
			.suffix = "txt",
			.infixes = NULL
		},
		.expected_out_files = NULL
	},
	{ NULL }
};

static void	globbing_basic_tests(void **case_name)
{
	t_test_case	*tc;
	t_list		*tested;
	t_error		error;
	int			equal;

	if (case_name == NULL)
		return (printf("Implementation error.\n"), assert_true(0));
	tested = NULL;
	tc = find_case(globbing_cases, *case_name);
	if (tc == NULL)
		return (printf(CASE_NOT_FOUND_MSG, (char *) *case_name), assert_true(0));
	error = globbing(&tested, tc->in_files, tc->patterns);
	equal = lst_equal(tested, tc->expected_out_files, (void *) str_equal);
	ft_lstclear(&tested, NULL);
	if (!equal || error != ERR_NONE)
		return(printf(FAIL_MSG, (char *) *case_name), assert_true(0));
	return (printf(SUCCESS_MSG, (char *) *case_name), assert_true(1));
}

t_test_result	globbing_tests(void)
{
	const struct CMUnitTest test_cases[] = {
		cmocka_unit_test_prestate(globbing_basic_tests, globbing_cases[0].name),
		cmocka_unit_test_prestate(globbing_basic_tests, globbing_cases[1].name),
		cmocka_unit_test_prestate(globbing_basic_tests, globbing_cases[2].name),
		cmocka_unit_test_prestate(globbing_basic_tests, globbing_cases[3].name),
		cmocka_unit_test_prestate(globbing_basic_tests, globbing_cases[4].name),
		cmocka_unit_test_prestate(globbing_basic_tests, globbing_cases[5].name),
		cmocka_unit_test_prestate(globbing_basic_tests, globbing_cases[6].name),
		cmocka_unit_test_prestate(globbing_basic_tests, globbing_cases[7].name),
		cmocka_unit_test_prestate(globbing_basic_tests, globbing_cases[8].name),
		cmocka_unit_test_prestate(globbing_basic_tests, globbing_cases[9].name),
		cmocka_unit_test_prestate(globbing_basic_tests, globbing_cases[10].name),
		cmocka_unit_test_prestate(globbing_basic_tests, globbing_cases[11].name),
		cmocka_unit_test_prestate(globbing_basic_tests, globbing_cases[12].name),
		cmocka_unit_test_prestate(globbing_basic_tests, globbing_cases[13].name),
		cmocka_unit_test_prestate(globbing_basic_tests, globbing_cases[14].name),
		cmocka_unit_test_prestate(globbing_basic_tests, globbing_cases[15].name),
		cmocka_unit_test_prestate(globbing_basic_tests, globbing_cases[16].name),
		cmocka_unit_test_prestate(globbing_basic_tests, globbing_cases[17].name),
		cmocka_unit_test_prestate(globbing_basic_tests, globbing_cases[18].name),
		cmocka_unit_test_prestate(globbing_basic_tests, globbing_cases[19].name),
		cmocka_unit_test_prestate(globbing_basic_tests, globbing_cases[20].name),
		cmocka_unit_test_prestate(globbing_basic_tests, globbing_cases[21].name),
		cmocka_unit_test_prestate(globbing_basic_tests, globbing_cases[22].name),
		cmocka_unit_test_prestate(globbing_basic_tests, globbing_cases[23].name),
		cmocka_unit_test_prestate(globbing_basic_tests, globbing_cases[24].name),
		cmocka_unit_test_prestate(globbing_basic_tests, globbing_cases[25].name),
		cmocka_unit_test_prestate(globbing_basic_tests, globbing_cases[26].name),
		cmocka_unit_test_prestate(globbing_basic_tests, globbing_cases[27].name),
		cmocka_unit_test_prestate(globbing_basic_tests, globbing_cases[28].name),
		cmocka_unit_test_prestate(globbing_basic_tests, globbing_cases[29].name),
		cmocka_unit_test_prestate(globbing_basic_tests, globbing_cases[30].name),
	};
	char			name[] = "lexer/globbing";
	t_test_result	result;

	printf(GROUP_HEADER, name);
	result.total = sizeof(test_cases)/sizeof(struct CMUnitTest);
	result.failed = cmocka_run_group_tests_name(name, test_cases, NULL, NULL);
	result.successful = result.total - result.failed;
	printf(GROUP_RESULT, name, result.successful, result.total);
	return (result);
}
