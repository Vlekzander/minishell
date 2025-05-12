/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing_tests.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:43:50 by apierret          #+#    #+#             */
/*   Updated: 2025/05/12 11:13:40 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "tests.h"
#include "test_utils.h"

static t_case globbing_cases[] =
{
	{
		.name = "no_file_no_pattern",
		.in_files = NULL,
		.patterns = &(t_glob_pattern){
			.prefix = NULL,
			.suffix = NULL,
			.infixes = NULL
		},
		.excepted_out_files = NULL
	},
	{
		.name = "file_no_pattern",
		.in_files = &(t_list) {
			.content = "file.txt",
			.next = NULL
		},
		.patterns = &(t_glob_pattern){
			.prefix = NULL,
			.suffix = NULL,
			.infixes = NULL
		},
		.excepted_out_files = &(t_list) {
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
		.patterns = &(t_glob_pattern){
			.prefix = NULL,
			.suffix = NULL,
			.infixes = NULL
		},
		.excepted_out_files = &(t_list) {
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
		.patterns = &(t_glob_pattern){
			.prefix = "file",
			.suffix = NULL,
			.infixes = NULL
		},
		.excepted_out_files = NULL
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
		.patterns = &(t_glob_pattern){
			.prefix = "file",
			.suffix = NULL,
			.infixes = NULL
		},
		.excepted_out_files = &(t_list) {
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
		.patterns = &(t_glob_pattern){
			.prefix = "file.txt",
			.suffix = NULL,
			.infixes = NULL
		},
		.excepted_out_files = &(t_list) {
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
		.patterns = &(t_glob_pattern){
			.prefix = "file",
			.suffix = NULL,
			.infixes = NULL
		},
		.excepted_out_files = &(t_list) {
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
		.patterns = &(t_glob_pattern){
			.prefix = "app",
			.suffix = NULL,
			.infixes = NULL
		},
		.excepted_out_files = NULL
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
		.patterns = &(t_glob_pattern){
			.prefix = "app",
			.suffix = NULL,
			.infixes = NULL
		},
		.excepted_out_files = NULL
	},
	{
		.name = "no_file_suffix",
		.in_files = NULL,
		.patterns = &(t_glob_pattern){
			.prefix = NULL,
			.suffix = ".txt",
			.infixes = NULL
		},
		.excepted_out_files = NULL
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
		.patterns = &(t_glob_pattern){
			.prefix = NULL,
			.suffix = ".txt",
			.infixes = NULL
		},
		.excepted_out_files = &(t_list) {
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
		.patterns = &(t_glob_pattern){
			.prefix = NULL,
			.suffix = "file.txt",
			.infixes = NULL
		},
		.excepted_out_files = &(t_list) {
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
		.patterns = &(t_glob_pattern){
			.prefix = NULL,
			.suffix = ".txt",
			.infixes = NULL
		},
		.excepted_out_files = &(t_list) {
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
		.patterns = &(t_glob_pattern){
			.prefix = NULL,
			.suffix = ".md",
			.infixes = NULL
		},
		.excepted_out_files = NULL
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
		.patterns = &(t_glob_pattern){
			.prefix = NULL,
			.suffix = ".md",
			.infixes = NULL
		},
		.excepted_out_files = NULL
	},
	{
		.name = "no_file_prefix_suffix",
		.in_files = NULL,
		.patterns = &(t_glob_pattern){
			.prefix = "fi",
			.suffix = ".txt",
			.infixes = NULL
		},
		.excepted_out_files = NULL
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
		.patterns = &(t_glob_pattern){
			.prefix = "fi",
			.suffix = ".txt",
			.infixes = NULL
		},
		.excepted_out_files = &(t_list) {
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
		.patterns = &(t_glob_pattern){
			.prefix = "file",
			.suffix = ".txt",
			.infixes = NULL
		},
		.excepted_out_files = &(t_list) {
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
		.patterns = &(t_glob_pattern){
			.prefix = "file",
			.suffix = ".txt",
			.infixes = NULL
		},
		.excepted_out_files = &(t_list) {
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
		.patterns = &(t_glob_pattern){
			.prefix = "doc",
			.suffix = ".md",
			.infixes = NULL
		},
		.excepted_out_files = NULL
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
		.patterns = &(t_glob_pattern){
			.prefix = "doc",
			.suffix = ".md",
			.infixes = NULL
		},
		.excepted_out_files = NULL
	},
	{
		.name = "no_file_infixe",
		.in_files = NULL,
		.patterns = &(t_glob_pattern){
			.prefix = NULL,
			.suffix = NULL,
			.infixes = &(t_list)
			{
				.content = "le.tx",
				.next = NULL
			}
		},
		.excepted_out_files = NULL
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
		.patterns = &(t_glob_pattern){
			.prefix = NULL,
			.suffix = NULL,
			.infixes = &(t_list)
			{
				.content = "le.tx",
				.next = NULL
			}
		},
		.excepted_out_files = &(t_list) {
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
		.patterns = &(t_glob_pattern){
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
		.excepted_out_files = &(t_list) {
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
		.patterns = &(t_glob_pattern){
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
		.excepted_out_files = &(t_list) {
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
		.patterns = &(t_glob_pattern){
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
		.excepted_out_files = NULL
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
		.patterns = &(t_glob_pattern){
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
		.excepted_out_files = NULL
	},
};

MunitResult	globbing_basic_tests(const MunitParameter params[], void* data)
{
	t_case	*tc;
	t_list	*tested;
	t_error	error;
	int		equal;

	(void) data;
	tested = NULL;
	tc = find_case(globbing_cases, munit_parameters_get(params, "case"));
	if (tc == NULL)
		return (munit_log(MUNIT_LOG_ERROR, "Test case not found"), MUNIT_ERROR);
	error = globbing(&tested, tc->in_files, tc->patterns);
	equal = lst_equal(tested, tc->excepted_out_files, (void *) str_equal);
	ft_lstclear(&tested, NULL);
	if (!equal || error != ERR_NONE)
		return (MUNIT_FAIL);
	return (MUNIT_OK);
}
