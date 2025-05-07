/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing_tests.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:43:50 by apierret          #+#    #+#             */
/*   Updated: 2025/05/07 13:15:17 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "tests.h"
#include "test_utils.h"
#include "utils.h"

static t_case globbing_cases[] =
{
	{
		.name = "no_file_no_pattern",
		.in_files = { NULL },
		.patterns = &(t_glob){
			.prefix = NULL,
			.suffix = NULL,
			.infixes = NULL
		},
		.excepted_out_files = { NULL }
	},
	{
		.name = "file_no_pattern",
		.in_files = { "file.txt", NULL },
		.patterns = &(t_glob){
			.prefix = NULL,
			.suffix = NULL,
			.infixes = NULL
		},
		.excepted_out_files = { "file.txt", NULL }
	},
	{
		.name = "mult_files_no_pattern",
		.in_files = { "file1.txt", "file2.txt", "file3.txt", NULL },
		.patterns = &(t_glob){
			.prefix = NULL,
			.suffix = NULL,
			.infixes = NULL
		},
		.excepted_out_files = { "file1.txt", "file2.txt", "file3.txt", NULL }
	},
	{
		.name = "no_file_prefix",
		.in_files = { NULL },
		.patterns = &(t_glob){
			.prefix = "file",
			.suffix = NULL,
			.infixes = NULL
		},
		.excepted_out_files = { NULL }
	},
	{
		.name = "file_prefix",
		.in_files = { "file.txt", "another.md", "something.jpg", NULL },
		.patterns = &(t_glob){
			.prefix = "file",
			.suffix = NULL,
			.infixes = NULL
		},
		.excepted_out_files = { "file.txt", NULL }
	},
	{
		.name = "file_full_prefix",
		.in_files = { "file.txt", "another.md", "something.jpg", NULL },
		.patterns = &(t_glob){
			.prefix = "file.txt",
			.suffix = NULL,
			.infixes = NULL
		},
		.excepted_out_files = { "file.txt", NULL }
	},
	{
		.name = "mult_files_prefix",
		.in_files = { "file1.txt", "file2.txt", "file3.txt", "another.md", "something.jpg", NULL },
		.patterns = &(t_glob){
			.prefix = "file",
			.suffix = NULL,
			.infixes = NULL
		},
		.excepted_out_files = { "file1.txt", "file2.txt", "file3.txt", NULL }
	},
	{
		.name = "file_prefix_bad",
		.in_files = { "file.txt", NULL },
		.patterns = &(t_glob){
			.prefix = "app",
			.suffix = NULL,
			.infixes = NULL
		},
		.excepted_out_files = { NULL }
	},
	{
		.name = "mult_files_prefix_bad",
		.in_files = { "file1.txt", "file2.txt", "file3.txt", NULL },
		.patterns = &(t_glob){
			.prefix = "app",
			.suffix = NULL,
			.infixes = NULL
		},
		.excepted_out_files = { NULL }
	},
	{
		.name = "no_file_suffix",
		.in_files = { NULL },
		.patterns = &(t_glob){
			.prefix = NULL,
			.suffix = ".txt",
			.infixes = NULL
		},
		.excepted_out_files = { NULL }
	},
	{
		.name = "file_suffix",
		.in_files = { "file.txt", "another.md", "something.jpg", NULL },
		.patterns = &(t_glob){
			.prefix = NULL,
			.suffix = ".txt",
			.infixes = NULL
		},
		.excepted_out_files = { "file.txt", NULL }
	},
	{
		.name = "file_full_suffix",
		.in_files = { "file.txt", "another.md", "something.jpg", NULL },
		.patterns = &(t_glob){
			.prefix = NULL,
			.suffix = "file.txt",
			.infixes = NULL
		},
		.excepted_out_files = { "file.txt", NULL }
	},
	{
		.name = "mult_files_suffix",
		.in_files = { "file1.txt", "file2.txt", "file3.txt", "another.md", "something.jpg", NULL },
		.patterns = &(t_glob){
			.prefix = NULL,
			.suffix = ".txt",
			.infixes = NULL
		},
		.excepted_out_files = { "file1.txt", "file2.txt", "file3.txt", NULL }
	},
	{
		.name = "file_suffix_bad",
		.in_files = { "file.txt" },
		.patterns = &(t_glob){
			.prefix = NULL,
			.suffix = ".md",
			.infixes = NULL
		},
		.excepted_out_files = { NULL }
	},
	{
		.name = "mult_files_suffix_bad",
		.in_files = { "file1.txt", "file2.txt", "file3.txt", NULL },
		.patterns = &(t_glob){
			.prefix = NULL,
			.suffix = ".md",
			.infixes = NULL
		},
		.excepted_out_files = { NULL }
	},
	{
		.name = "no_file_prefix_suffix",
		.in_files = { NULL },
		.patterns = &(t_glob){
			.prefix = "fi",
			.suffix = ".txt",
			.infixes = NULL
		},
		.excepted_out_files = { NULL }
	},
	{
		.name = "file_prefix_suffix",
		.in_files = { "file.txt", "another.md", "something.jpg", "filename.md", NULL },
		.patterns = &(t_glob){
			.prefix = "fi",
			.suffix = ".txt",
			.infixes = NULL
		},
		.excepted_out_files = { "file.txt", NULL }
	},
	{
		.name = "file_full_prefix_suffix",
		.in_files = { "file.txt", "another.md", "something.jpg", NULL },
		.patterns = &(t_glob){
			.prefix = "file",
			.suffix = ".txt",
			.infixes = NULL
		},
		.excepted_out_files = { "file.txt", NULL }
	},
	{
		.name = "mult_files_prefix_suffix",
		.in_files = { "file1.txt", "file2.txt", "file3.txt", "another.md", "something.jpg", NULL },
		.patterns = &(t_glob){
			.prefix = "file",
			.suffix = ".txt",
			.infixes = NULL
		},
		.excepted_out_files = { "file1.txt", "file2.txt", "file3.txt", NULL }
	},
	{
		.name = "file_prefix_suffix_bad",
		.in_files = { "file.txt", NULL },
		.patterns = &(t_glob){
			.prefix = "doc",
			.suffix = ".md",
			.infixes = NULL
		},
		.excepted_out_files = { NULL }
	},
	{
		.name = "mult_files_prefix_suffix_bad",
		.in_files = { "file1.txt", "file2.txt", "file3.txt", NULL },
		.patterns = &(t_glob){
			.prefix = "doc",
			.suffix = ".md",
			.infixes = NULL
		},
		.excepted_out_files = { NULL }
	},
	{
		.name = "no_file_infixe",
		.in_files = { NULL },
		.patterns = &(t_glob){
			.prefix = NULL,
			.suffix = NULL,
			.infixes = &(t_list)
			{
				.content = "le.tx",
				.next = NULL
			}
		},
		.excepted_out_files = { NULL }
	},
	{
		.name = "file_infixe",
		.in_files = { "file.txt", "another.md", "something.jpg", NULL },
		.patterns = &(t_glob){
			.prefix = NULL,
			.suffix = NULL,
			.infixes = &(t_list)
			{
				.content = "le.tx",
				.next = NULL
			}
		},
		.excepted_out_files = { "file.txt", NULL }
	},
	{
		.name = "file_full_infixe",
		.in_files = { "file.txt", "another.md", "something.jpg", NULL },
		.patterns = &(t_glob){
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
		.excepted_out_files = { "file.txt", NULL }
	},
	{
		.name = "mult_files_infixe",
		.in_files = { "file1.txt", "file2.txt", "file3.txt", "another.md", "something.jpg", NULL },
		.patterns = &(t_glob){
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
		.excepted_out_files = { "file1.txt", "file2.txt", "file3.txt", NULL }
	},
	{
		.name = "file_infixe_bad",
		.in_files = { "file.txt", NULL },
		.patterns = &(t_glob){
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
		.excepted_out_files = { NULL }
	},
	{
		.name = "mult_files_infixe_bad",
		.in_files = { "file1.txt", "file2.txt", "file3.txt", NULL },
		.patterns = &(t_glob){
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
		.excepted_out_files = { NULL }
	},
};

MunitResult	globbing_basic_tests(const MunitParameter params[], void* data)
{
	t_case	*tc;
	char	**tested;
	t_error	error;
	int		equal;

	(void) data;
	tested = NULL;
	tc = find_case(globbing_cases, munit_parameters_get(params, "case"));
	if (tc == NULL)
		return (munit_log(MUNIT_LOG_ERROR, "Test case not found"), MUNIT_ERROR);
	error = globbing(&tested, tc->in_files, tc->patterns);
	equal = str_array_equal(tc->excepted_out_files, tested);
	free_ddarray((void **) tested);
	if (!equal || error != ERR_NONE)
		return (MUNIT_FAIL);
	return (MUNIT_OK);
}
