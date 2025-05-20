/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tests.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 12:14:40 by apierret          #+#    #+#             */
/*   Updated: 2025/05/20 11:44:21 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "lexer.h"
#include "test.h"
#include "test_utils.h"

static t_test_case expand_cases[] =
{
	{
		.name = "no_expand",
		.input_tokens = { "echo", "hello", "world", NULL },
		.extracted_pattern_expand = { NULL, NULL, NULL },
		.filtered_files_expand = { NULL, NULL, NULL },
		.expected_tokens_expand = { "echo", "hello", "world", NULL },
	},
	{
		.name = "no_match",
		.input_tokens = { "cat", "*.md", NULL },
		.extracted_pattern_expand = {
			NULL,
			&(t_pattern) {
				.prefix = NULL,
				.infixes = NULL,
				.suffix = ".md"
			}
		},
		.filtered_files_expand = { NULL, NULL },
		.expected_tokens_expand = { "cat", "*.md", NULL },
	},
	{
		.name = "simple_match",
		.input_tokens = { "ls", "*.txt", NULL },
		.extracted_pattern_expand = {
			NULL,
			&(t_pattern) {
				.prefix = NULL,
				.infixes = NULL,
				.suffix = ".txt"
			}
		},
		.filtered_files_expand = {
			NULL,
			&(t_list){
				.content = "a.txt",
				.next = &(t_list){
					.content = "b.txt",
					.next = NULL
				}
			}
		},
		.expected_tokens_expand = { "ls", "a.txt", "b.txt", NULL },
	},
	{
		.name = "wildcard_middle",
		.input_tokens = { "rm", "test*.log", NULL },
		.extracted_pattern_expand = {
			NULL,
			&(t_pattern) {
				.prefix = "test",
				.infixes = NULL,
				.suffix = ".log"
			}
		},
		.filtered_files_expand = {
			NULL,
			&(t_list){
				.content = "test1.log",
				.next = &(t_list){
					.content = "test2.log",
					.next = NULL
				}
			}
		},
		.expected_tokens_expand = { "rm", "test1.log", "test2.log", NULL },
	},
	{
		.name = "multiple_wildcards",
		.input_tokens = { "cp", "*.c", "*.h", "backup/", NULL },
		.extracted_pattern_expand = {
			NULL,
			&(t_pattern) {
				.prefix = NULL,
				.infixes = NULL,
				.suffix = ".c"
			},
			&(t_pattern) {
				.prefix = NULL,
				.infixes = NULL,
				.suffix = ".h"
			},
			NULL,
		},
		.filtered_files_expand = {
			NULL,
			&(t_list){
				.content = "main.c",
				.next = &(t_list){
					.content = "utils.c",
					.next = NULL
				}
			},
			&(t_list){
				.content = "utils.h",
				.next = NULL
			},
			NULL,
		},
		.expected_tokens_expand = { "cp", "main.c", "utils.c", "utils.h", "backup/", NULL },
	},
	{
		.name = "match_all_files",
		.input_tokens = { "echo", "*", NULL },
		.extracted_pattern_expand = {
			NULL,
			&(t_pattern) {
				.prefix = NULL,
				.infixes = NULL,
				.suffix = NULL
			}
		},
		.filtered_files_expand = {
			NULL,
			&(t_list){
				.content = "a.txt",
				.next = &(t_list){
					.content = "b.md",
					.next = &(t_list){
						.content = "c.pdf",
						.next = NULL
					}
				}
			}
		},
		.expected_tokens_expand = { "echo", "a.txt", "b.md", "c.pdf", NULL },
	},
	{
		.name = "wildcard_with_dot",
		.input_tokens = { "cat", "*.*", NULL },
		.extracted_pattern_expand = {
			NULL,
			&(t_pattern) {
				.prefix = NULL,
				.infixes = &(t_list)
				{
					.content = ".",
					.next = NULL
				},
				.suffix = NULL
			}
		},
		.filtered_files_expand = {
			NULL,
			&(t_list){
				.content = "main.c",
				.next = &(t_list){
					.content = "readme.md",
					.next = NULL
				}
			}
		},
		.expected_tokens_expand = { "cat", "main.c", "readme.md", NULL },
	},
	{
		.name = "hidden_files",
		.input_tokens = { "echo", ".*", NULL },
		.extracted_pattern_expand = {
			NULL,
			&(t_pattern) {
				.prefix = ".",
				.infixes = NULL,
				.suffix = NULL
			}
		},
		.filtered_files_expand = {
			NULL,
			&(t_list){
				.content = ".bash_history",
				.next = &(t_list){
					.content = ".cache",
					.next = &(t_list){
						.content = ".config",
						.next = NULL
					}
				}
			}
		},
		.expected_tokens_expand = { "echo", ".bash_history", ".cache", ".config", NULL },
	},
	{
		.name = "empty_directory",
		.input_tokens = { "echo", "*", NULL },
		.extracted_pattern_expand = {
			NULL,
			&(t_pattern) {
				.prefix = NULL,
				.infixes = NULL,
				.suffix = NULL
			}
		},
		.filtered_files_expand = {
			NULL,
			NULL
		},
		.expected_tokens_expand = { "echo", "*", NULL },
	},
	{
		.name = "prefix_only",
		.input_tokens = { "echo", "main*", NULL },
		.extracted_pattern_expand = {
			NULL,
			&(t_pattern) {
				.prefix = "main",
				.infixes = NULL,
				.suffix = NULL
			}
		},
		.filtered_files_expand = {
			NULL,
			&(t_list){
				.content = "main.c",
				.next = &(t_list){
					.content = "main.h",
					.next = &(t_list){
						.content = "main_backup.c",
						.next = NULL
					}
				}
			}
		},
		.expected_tokens_expand = { "echo", "main.c", "main.h", "main_backup.c", NULL },
	},
	{
		.name = "suffix_only",
		.input_tokens = { "echo", "*.c", NULL },
		.extracted_pattern_expand = {
			NULL,
			&(t_pattern) {
				.prefix = NULL,
				.infixes = NULL,
				.suffix = ".c"
			}
		},
		.filtered_files_expand = {
			NULL,
			&(t_list){
				.content = "main.c",
				.next = &(t_list){
					.content = "tests.c",
					.next = &(t_list){
						.content = "main_backup.c",
						.next = NULL
					}
				}
			}
		},
		.expected_tokens_expand = { "echo", "main.c", "tests.c", "main_backup.c", NULL },
	},
	{
		.name = "literal_asterisk",
		.input_tokens = { "echo", "file*.txt", NULL },
		.extracted_pattern_expand = {
			NULL,
			&(t_pattern) {
				.prefix = "file",
				.infixes = NULL,
				.suffix = ".txt"
			}
		},
		.filtered_files_expand = {
			NULL,
			&(t_list){
				.content = "file*.txt",
				.next = &(t_list){
					.content = "file1.txt",
					.next = &(t_list){
						.content = "file2.txt",
						.next = NULL
					}
				}
			}
		},
		.expected_tokens_expand = { "echo", "file*.txt", "file1.txt", "file2.txt", NULL },
	},
	{
		.name = "file_with_space",
		.input_tokens = { "echo", "file*.txt", NULL },
		.extracted_pattern_expand = {
			NULL,
			&(t_pattern) {
				.prefix = "file",
				.infixes = NULL,
				.suffix = ".txt"
			}
		},
		.filtered_files_expand = {
			NULL,
			&(t_list){
				.content = "file 1.txt",
				.next = &(t_list){
					.content = "file 2.txt",
					.next = &(t_list){
						.content = "file 3.txt",
						.next = NULL
					}
				}
			}
		},
		.expected_tokens_expand = { "echo", "file 1.txt", "file 2.txt", "file 3.txt", NULL },
	},
	{ NULL }
};

t_error	__wrap_extract_pattern(t_pattern **pattern, char *str)
{
	(void) str;
	*pattern = mock_type(t_pattern *);
	return (mock_type(t_error));
}

t_error	__wrap_globbing(t_list **out_files, t_list *in_files, t_pattern *pattern)
{
	(void) in_files;
	(void) pattern;
	*out_files = mock_type(t_list *);
	return (mock_type(t_error));
}

static void expand_basic_tests(void **case_name)
{
	t_test_case	*tc;
	t_list		*tested;
	size_t		size;
	t_list		*expected;
	t_error		error;
	int			equal;

	if (case_name == NULL)
		return (printf("Implementation error.\n"), assert_true(0));
	tested = NULL;
	expected = NULL;
	tc = find_case(expand_cases, *case_name);
	if (tc == NULL)
		return (printf(CASE_NOT_FOUND_MSG, (char *) *case_name), assert_true(0));
	tested = create_token_list(tc->input_tokens);
	expected = create_token_list(tc->expected_tokens_expand);
	size = ft_lstsize(tested);
	for (size_t i = 0; i < size; i++)
	{
		if (tc->extracted_pattern_expand[i] != NULL)
		{
			will_return(__wrap_extract_pattern, tc->extracted_pattern_expand[i]);
			will_return(__wrap_extract_pattern, ERR_NONE);
		}
		if (tc->filtered_files_expand[i] != NULL)
		{
			will_return(__wrap_globbing, tc->filtered_files_expand[i]);
			will_return(__wrap_globbing, ERR_NONE);
		}
	}
	error = expand(&tested);
	equal = lst_equal(expected, tested, (void *) token_equal);
	ft_lstclear(&tested, (void *) free_token);
	ft_lstclear(&expected, (void *) free_token);
	if (!equal || error != ERR_NONE)
		return(printf(FAIL_MSG, (char *) *case_name), assert_true(0));
	return (printf(SUCCESS_MSG, (char *) *case_name), assert_true(1));
}

t_test_result	execute_tests(void)
{
	const struct CMUnitTest test_cases[] = {
		cmocka_unit_test_prestate(expand_basic_tests, expand_cases[0].name),
		cmocka_unit_test_prestate(expand_basic_tests, expand_cases[1].name),
		cmocka_unit_test_prestate(expand_basic_tests, expand_cases[2].name),
		cmocka_unit_test_prestate(expand_basic_tests, expand_cases[3].name),
		cmocka_unit_test_prestate(expand_basic_tests, expand_cases[4].name),
		cmocka_unit_test_prestate(expand_basic_tests, expand_cases[5].name),
		cmocka_unit_test_prestate(expand_basic_tests, expand_cases[6].name),
		cmocka_unit_test_prestate(expand_basic_tests, expand_cases[7].name),
		cmocka_unit_test_prestate(expand_basic_tests, expand_cases[8].name),
		cmocka_unit_test_prestate(expand_basic_tests, expand_cases[9].name),
		cmocka_unit_test_prestate(expand_basic_tests, expand_cases[10].name),
		cmocka_unit_test_prestate(expand_basic_tests, expand_cases[11].name),
		cmocka_unit_test_prestate(expand_basic_tests, expand_cases[12].name),
	};
	char			name[] = "lexer/expand";
	t_test_result	result;

	printf(GROUP_HEADER, name);
	result.total = sizeof(test_cases)/sizeof(struct CMUnitTest);
	result.failed = cmocka_run_group_tests_name(name, test_cases, NULL, NULL);
	result.successful = result.total - result.failed;
	printf(GROUP_RESULT, name, result.successful, result.total);
	return (result);
}
