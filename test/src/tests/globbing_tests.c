/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing_tests.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 22:31:40 by apierret          #+#    #+#             */
/*   Updated: 2025/06/07 18:49:49 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "expand.h"
#include "test.h"
#include "test_utils.h"

static t_test_case globbing_cases[] =
{
	{
		"no_file_no_pattern",
		&(t_globbing_case)
		{
			.input_files = NULL,
			.pattern = &(t_pattern){
				.prefix = NULL,
				.suffix = NULL,
				.infixes = NULL
			},
			.expected_out_files = NULL
		}
	},
	{
		"file_no_pattern",
		&(t_globbing_case)
		{
			.input_files = &(t_list){
				.content = "file.txt",
				.next = NULL
			},
			.pattern = &(t_pattern){
				.prefix = NULL,
				.suffix = NULL,
				.infixes = NULL
			},
			.expected_out_files = &(t_list){
				.content = "file.txt",
				.next = NULL
			},
		}
	},
	{
		"mult_files_no_pattern",
		&(t_globbing_case)
		{
			.input_files = &(t_list){
				.content = "file1.txt",
				.next = &(t_list){
					.content = "file2.txt",
					.next = &(t_list){
						.content = "file3.txt",
						.next = NULL
					},
				},
			},
			.pattern = &(t_pattern){
				.prefix = NULL,
				.suffix = NULL,
				.infixes = NULL
			},
			.expected_out_files = &(t_list){
				.content = "file1.txt",
				.next = &(t_list){
					.content = "file2.txt",
					.next = &(t_list){
						.content = "file3.txt",
						.next = NULL
					},
				},
			},
		}
	},
	{
		"no_file_prefix",
		&(t_globbing_case)
		{
			.input_files = NULL,
			.pattern = &(t_pattern){
				.prefix = "file",
				.suffix = NULL,
				.infixes = NULL
			},
			.expected_out_files = NULL
		}
	},
	{
		"file_prefix",
		&(t_globbing_case)
		{
			.input_files = &(t_list){
				.content = "file.txt",
				.next = &(t_list){
					.content = "another.md",
					.next = &(t_list){
						.content = "something.jpg",
						.next = NULL
					},
				},
			},
			.pattern = &(t_pattern){
				.prefix = "file",
				.suffix = NULL,
				.infixes = NULL
			},
			.expected_out_files = &(t_list){
				.content = "file.txt",
				.next = NULL
			},
		}
	},
	{
		"file_full_prefix",
		&(t_globbing_case)
		{
			.input_files = &(t_list){
				.content = "file.txt",
				.next = &(t_list){
					.content = "another.md",
					.next = &(t_list){
						.content = "something.jpg",
						.next = NULL
					},
				},
			},
			.pattern = &(t_pattern){
				.prefix = "file.txt",
				.suffix = NULL,
				.infixes = NULL
			},
			.expected_out_files = &(t_list){
				.content = "file.txt",
				.next = NULL
			},
		}
	},
	{
		"mult_files_prefix",
		&(t_globbing_case)
		{
			.input_files = &(t_list){
				.content = "file1.txt",
				.next = &(t_list){
					.content = "file2.txt",
					.next = &(t_list){
						.content = "file3.txt",
						.next = &(t_list){
							.content = "another.md",
							.next = &(t_list){
								.content = "something.jpg",
								.next = NULL
							},
						},
					},
				},
			},
			.pattern = &(t_pattern){
				.prefix = "file",
				.suffix = NULL,
				.infixes = NULL
			},
			.expected_out_files = &(t_list){
				.content = "file1.txt",
				.next = &(t_list){
					.content = "file2.txt",
					.next = &(t_list){
						.content = "file3.txt",
						.next = NULL
					},
				},
			},
		}
	},
	{
		"file_prefix_bad",
		&(t_globbing_case)
		{
			.input_files = &(t_list){
				.content = "file.txt",
				.next = NULL
			},
			.pattern = &(t_pattern){
				.prefix = "app",
				.suffix = NULL,
				.infixes = NULL
			},
			.expected_out_files = NULL
		}
	},
	{
		"mult_files_prefix_bad",
		&(t_globbing_case)
		{
			.input_files = &(t_list){
				.content = "file1.txt",
				.next = &(t_list){
					.content = "file2.txt",
					.next = &(t_list){
						.content = "file3.txt",
						.next = NULL
					},
				},
			},
			.pattern = &(t_pattern){
				.prefix = "app",
				.suffix = NULL,
				.infixes = NULL
			},
			.expected_out_files = NULL
		}
	},
	{
		"no_file_suffix",
		&(t_globbing_case)
		{
			.input_files = NULL,
			.pattern = &(t_pattern){
				.prefix = NULL,
				.suffix = ".txt",
				.infixes = NULL
			},
			.expected_out_files = NULL
		}
	},
	{
		"file_suffix",
		&(t_globbing_case)
		{
			.input_files = &(t_list){
				.content = "file.txt",
				.next = &(t_list){
					.content = "another.md",
					.next = &(t_list){
						.content = "something.jpg",
						.next = NULL
					},
				},
			},
			.pattern = &(t_pattern){
				.prefix = NULL,
				.suffix = ".txt",
				.infixes = NULL
			},
			.expected_out_files = &(t_list){
				.content = "file.txt",
				.next = NULL
			},
		}
	},
	{
		"file_full_suffix",
		&(t_globbing_case)
		{
			.input_files = &(t_list){
				.content = "file.txt",
				.next = &(t_list){
					.content = "another.md",
					.next = &(t_list){
						.content = "something.jpg",
						.next = NULL
					},
				},
			},
			.pattern = &(t_pattern){
				.prefix = NULL,
				.suffix = "file.txt",
				.infixes = NULL
			},
			.expected_out_files = &(t_list){
				.content = "file.txt",
				.next = NULL
			},
		}
	},
	{
		"mult_files_suffix",
		&(t_globbing_case)
		{
			.input_files = &(t_list){
				.content = "file1.txt",
				.next = &(t_list){
					.content = "file2.txt",
					.next = &(t_list){
						.content = "file3.txt",
						.next = &(t_list){
							.content = "another.md",
							.next = &(t_list){
								.content = "something.jpg",
								.next = NULL
							},
						},
					},
				},
			},
			.pattern = &(t_pattern){
				.prefix = NULL,
				.suffix = ".txt",
				.infixes = NULL
			},
			.expected_out_files = &(t_list){
				.content = "file1.txt",
				.next = &(t_list){
					.content = "file2.txt",
					.next = &(t_list){
						.content = "file3.txt",
						.next = NULL
					},
				},
			},
		}
	},
	{
		"file_suffix_bad",
		&(t_globbing_case)
		{
			.input_files = &(t_list){
				.content = "file.txt",
				.next = NULL
			},
			.pattern = &(t_pattern){
				.prefix = NULL,
				.suffix = ".md",
				.infixes = NULL
			},
			.expected_out_files = NULL
		}
	},
	{
		"mult_files_suffix_bad",
		&(t_globbing_case)
		{
			.input_files = &(t_list){
				.content = "file1.txt",
				.next = &(t_list){
					.content = "file2.txt",
					.next = &(t_list){
						.content = "file3.txt",
						.next = NULL
					},
				},
			},
			.pattern = &(t_pattern){
				.prefix = NULL,
				.suffix = ".md",
				.infixes = NULL
			},
			.expected_out_files = NULL
		}
	},
	{
		"no_file_prefix_suffix",
		&(t_globbing_case)
		{
			.input_files = NULL,
			.pattern = &(t_pattern){
				.prefix = "fi",
				.suffix = ".txt",
				.infixes = NULL
			},
			.expected_out_files = NULL
		}
	},
	{
		"file_prefix_suffix",
		&(t_globbing_case)
		{
			.input_files = &(t_list){
				.content = "file.txt",
				.next = &(t_list){
					.content = "another.md",
					.next = &(t_list){
						.content = "something.jpg",
						.next = &(t_list){
							.content = "filename.md",
							.next = NULL
						},
					},
				},
			},
			.pattern = &(t_pattern){
				.prefix = "fi",
				.suffix = ".txt",
				.infixes = NULL
			},
			.expected_out_files = &(t_list){
				.content = "file.txt",
				.next = NULL
			},
		}
	},
	{
		"file_full_prefix_suffix",
		&(t_globbing_case)
		{
			.input_files = &(t_list){
				.content = "file.txt",
				.next = &(t_list){
					.content = "another.md",
					.next = &(t_list){
						.content = "something.jpg",
						.next = NULL
					},
				},
			},
			.pattern = &(t_pattern){
				.prefix = "file",
				.suffix = ".txt",
				.infixes = NULL
			},
			.expected_out_files = &(t_list){
				.content = "file.txt",
				.next = NULL
			},
		}
	},
	{
		"mult_files_prefix_suffix",
		&(t_globbing_case)
		{
			.input_files = &(t_list){
				.content = "file1.txt",
				.next = &(t_list){
					.content = "file2.txt",
					.next = &(t_list){
						.content = "file3.txt",
						.next = &(t_list){
							.content = "another.md",
							.next = &(t_list){
								.content = "something.jpg",
								.next = NULL
							},
						},
					},
				},
			},
			.pattern = &(t_pattern){
				.prefix = "file",
				.suffix = ".txt",
				.infixes = NULL
			},
			.expected_out_files = &(t_list){
				.content = "file1.txt",
				.next = &(t_list){
					.content = "file2.txt",
					.next = &(t_list){
						.content = "file3.txt",
						.next = NULL
					},
				},
			},
		}
	},
	{
		"file_prefix_suffix_bad",
		&(t_globbing_case)
		{
			.input_files = &(t_list){
				.content = "file.txt",
				.next = NULL
			},
			.pattern = &(t_pattern){
				.prefix = "doc",
				.suffix = ".md",
				.infixes = NULL
			},
			.expected_out_files = NULL
		}
	},
	{
		"mult_files_prefix_suffix_bad",
		&(t_globbing_case)
		{
			.input_files = &(t_list){
				.content = "file1.txt",
				.next = &(t_list){
					.content = "file2.txt",
					.next = &(t_list){
						.content = "file3.txt",
						.next = NULL
					},
				},
			},
			.pattern = &(t_pattern){
				.prefix = "doc",
				.suffix = ".md",
				.infixes = NULL
			},
			.expected_out_files = NULL
		}
	},
	{
		"no_file_infixe",
		&(t_globbing_case)
		{
			.input_files = NULL,
			.pattern = &(t_pattern){
				.prefix = NULL,
				.suffix = NULL,
				.infixes = &(t_list)
				{
					.content = "le.tx",
					.next = NULL
				}
			},
			.expected_out_files = NULL
		}
	},
	{
		"file_infixe",
		&(t_globbing_case)
		{
			.input_files = &(t_list){
				.content = "file.txt",
				.next = &(t_list){
					.content = "another.md",
					.next = &(t_list){
						.content = "something.jpg",
						.next = NULL
					},
				},
			},
			.pattern = &(t_pattern){
				.prefix = NULL,
				.suffix = NULL,
				.infixes = &(t_list)
				{
					.content = "le.tx",
					.next = NULL
				}
			},
			.expected_out_files = &(t_list){
				.content = "file.txt",
				.next = NULL
			},
		}
	},
	{
		"file_full_infixe",
		&(t_globbing_case)
		{
			.input_files = &(t_list){
				.content = "file.txt",
				.next = &(t_list){
					.content = "another.md",
					.next = &(t_list){
						.content = "something.jpg",
						.next = NULL
					},
				},
			},
			.pattern = &(t_pattern){
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
			.expected_out_files = &(t_list){
				.content = "file.txt",
				.next = NULL
			},
		}
	},
	{
		"mult_files_infixe",
		&(t_globbing_case)
		{
			.input_files = &(t_list){
				.content = "file1.txt",
				.next = &(t_list){
					.content = "file2.txt",
					.next = &(t_list){
						.content = "file3.txt",
						.next = &(t_list){
							.content = "another.md",
							.next = &(t_list){
								.content = "something.jpg",
								.next = NULL
							},
						},
					},
				},
			},
			.pattern = &(t_pattern){
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
			.expected_out_files = &(t_list){
				.content = "file1.txt",
				.next = &(t_list){
					.content = "file2.txt",
					.next = &(t_list){
						.content = "file3.txt",
						.next = NULL
					},
				},
			},
		}
	},
	{
		"file_infixe_bad",
		&(t_globbing_case)
		{
			.input_files = &(t_list){
				.content = "file.txt",
				.next = NULL
			},
			.pattern = &(t_pattern){
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
		}
	},
	{
		"mult_files_infixe_bad",
		&(t_globbing_case)
		{
			.input_files = &(t_list){
				.content = "file1.txt",
				.next = &(t_list){
					.content = "file2.txt",
					.next = &(t_list){
						.content = "file3.txt",
						.next = NULL
					},
				},
			},
			.pattern = &(t_pattern){
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
		}
	},
	{
		"any_with_hidden_files",
		&(t_globbing_case)
		{
			.input_files = &(t_list){
				.content = "file1.txt",
				.next = &(t_list){
					.content = "file2.txt",
					.next = &(t_list){
						.content = "file3.txt",
						.next = &(t_list){
							.content = ".hidden1.txt",
							.next = &(t_list){
								.content = ".hidden2.txt",
								.next = NULL
							},
						},
					},
				},
			},
			.pattern = &(t_pattern){
				.prefix = NULL,
				.suffix = NULL,
				.infixes = NULL
			},
			.expected_out_files = &(t_list){
				.content = "file1.txt",
				.next = &(t_list){
					.content = "file2.txt",
					.next = &(t_list){
						.content = "file3.txt",
						.next = NULL
					},
				},
			},
		}
	},
	{
		"prefix_hidden_files",
		&(t_globbing_case)
		{
			.input_files = &(t_list){
				.content = "file1.txt",
				.next = &(t_list){
					.content = "file2.txt",
					.next = &(t_list){
						.content = "file3.txt",
						.next = &(t_list){
							.content = ".hidden1.txt",
							.next = &(t_list){
								.content = ".hidden2.txt",
								.next = NULL
							},
						},
					},
				},
			},
			.pattern = &(t_pattern){
				.prefix = ".",
				.suffix = NULL,
				.infixes = NULL
			},
			.expected_out_files = &(t_list){
				.content = ".hidden1.txt",
				.next = &(t_list){
					.content = ".hidden2.txt",
					.next = NULL
				},
			},
		}
	},
	{
		"prefix_not_prefix",
		&(t_globbing_case)
		{
			.input_files = &(t_list){
				.content = "file1.txt",
				.next = &(t_list){
					.content = "file2.txt",
					.next = &(t_list){
						.content = "file3.txt",
						.next = NULL
					},
				},
			},
			.pattern = &(t_pattern){
				.prefix = "txt",
				.suffix = NULL,
				.infixes = NULL
			},
			.expected_out_files = NULL
		}
	},
	{
		"suffix_not_suffix",
		&(t_globbing_case)
		{
			.input_files = &(t_list){
				.content = "file1.txtfile",
				.next = &(t_list){
					.content = "file2.txtfile",
					.next = &(t_list){
						.content = "file3.txtfile",
						.next = NULL
					},
				},
			},
			.pattern = &(t_pattern){
				.prefix = NULL,
				.suffix = "txt",
				.infixes = NULL
			},
			.expected_out_files = NULL
		}
	},
	{NULL}
};

static t_list	*expected = NULL;
static t_list	*tested = NULL;

static int	test_setup(void **state) {
	(void) state;
	expected = NULL;
	tested = NULL;
	return (0);
}

static int	test_teardown(void **state) {
	(void) state;
	ft_lstclear(&tested, free);
	return (0);
}

static void	globbing_basic_tests(void **case_name)
{
	t_test_case		*tc;
	t_globbing_case	*data;
	t_error			error;
	int				equal;

	if (case_name == NULL)
		return(printf("Implementation error.\n"), assert_true(0));
	tc = find_case(globbing_cases, *case_name);
	if (tc == NULL)
		return (printf(CASE_NOT_FOUND_MSG, (char *) *case_name), assert_true(0));
	data = tc->data;
	expected = data->expected_out_files;
	error = globbing(&tested, data->input_files, data->pattern);
	equal = lst_equal(tested, expected, (void *) str_equal);
	if (error != ERR_NONE)
		return(printf(FAIL_MSG, (char *) *case_name, "error"), assert_true(0));
	if (!equal)
		return(printf(FAIL_MSG, (char *) *case_name, "not equal"), assert_true(0));
	return (printf(SUCCESS_MSG, (char *) *case_name), assert_true(1));
}

t_test_result	execute_tests(void)
{
	const struct CMUnitTest test_cases[] = {
		cmocka_unit_test_prestate_setup_teardown(globbing_basic_tests, test_setup, test_teardown, globbing_cases[0].name),
		cmocka_unit_test_prestate_setup_teardown(globbing_basic_tests, test_setup, test_teardown, globbing_cases[1].name),
		cmocka_unit_test_prestate_setup_teardown(globbing_basic_tests, test_setup, test_teardown, globbing_cases[2].name),
		cmocka_unit_test_prestate_setup_teardown(globbing_basic_tests, test_setup, test_teardown, globbing_cases[3].name),
		cmocka_unit_test_prestate_setup_teardown(globbing_basic_tests, test_setup, test_teardown, globbing_cases[4].name),
		cmocka_unit_test_prestate_setup_teardown(globbing_basic_tests, test_setup, test_teardown, globbing_cases[5].name),
		cmocka_unit_test_prestate_setup_teardown(globbing_basic_tests, test_setup, test_teardown, globbing_cases[6].name),
		cmocka_unit_test_prestate_setup_teardown(globbing_basic_tests, test_setup, test_teardown, globbing_cases[7].name),
		cmocka_unit_test_prestate_setup_teardown(globbing_basic_tests, test_setup, test_teardown, globbing_cases[8].name),
		cmocka_unit_test_prestate_setup_teardown(globbing_basic_tests, test_setup, test_teardown, globbing_cases[9].name),
		cmocka_unit_test_prestate_setup_teardown(globbing_basic_tests, test_setup, test_teardown, globbing_cases[10].name),
		cmocka_unit_test_prestate_setup_teardown(globbing_basic_tests, test_setup, test_teardown, globbing_cases[11].name),
		cmocka_unit_test_prestate_setup_teardown(globbing_basic_tests, test_setup, test_teardown, globbing_cases[12].name),
		cmocka_unit_test_prestate_setup_teardown(globbing_basic_tests, test_setup, test_teardown, globbing_cases[13].name),
		cmocka_unit_test_prestate_setup_teardown(globbing_basic_tests, test_setup, test_teardown, globbing_cases[14].name),
		cmocka_unit_test_prestate_setup_teardown(globbing_basic_tests, test_setup, test_teardown, globbing_cases[15].name),
		cmocka_unit_test_prestate_setup_teardown(globbing_basic_tests, test_setup, test_teardown, globbing_cases[16].name),
		cmocka_unit_test_prestate_setup_teardown(globbing_basic_tests, test_setup, test_teardown, globbing_cases[17].name),
		cmocka_unit_test_prestate_setup_teardown(globbing_basic_tests, test_setup, test_teardown, globbing_cases[18].name),
		cmocka_unit_test_prestate_setup_teardown(globbing_basic_tests, test_setup, test_teardown, globbing_cases[19].name),
		cmocka_unit_test_prestate_setup_teardown(globbing_basic_tests, test_setup, test_teardown, globbing_cases[20].name),
		cmocka_unit_test_prestate_setup_teardown(globbing_basic_tests, test_setup, test_teardown, globbing_cases[21].name),
		cmocka_unit_test_prestate_setup_teardown(globbing_basic_tests, test_setup, test_teardown, globbing_cases[22].name),
		cmocka_unit_test_prestate_setup_teardown(globbing_basic_tests, test_setup, test_teardown, globbing_cases[23].name),
		cmocka_unit_test_prestate_setup_teardown(globbing_basic_tests, test_setup, test_teardown, globbing_cases[24].name),
		cmocka_unit_test_prestate_setup_teardown(globbing_basic_tests, test_setup, test_teardown, globbing_cases[25].name),
		cmocka_unit_test_prestate_setup_teardown(globbing_basic_tests, test_setup, test_teardown, globbing_cases[26].name),
		cmocka_unit_test_prestate_setup_teardown(globbing_basic_tests, test_setup, test_teardown, globbing_cases[27].name),
		cmocka_unit_test_prestate_setup_teardown(globbing_basic_tests, test_setup, test_teardown, globbing_cases[28].name),
		cmocka_unit_test_prestate_setup_teardown(globbing_basic_tests, test_setup, test_teardown, globbing_cases[29].name),
		cmocka_unit_test_prestate_setup_teardown(globbing_basic_tests, test_setup, test_teardown, globbing_cases[30].name),
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
