/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tests.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 12:14:40 by apierret          #+#    #+#             */
/*   Updated: 2025/06/03 11:57:45 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "test.h"
#include "test_utils.h"

static t_test_case expand_cases[] =
{
	{
		.name = "no_expand",
		.input_tokens = { "echo", "hello", "world", NULL },
		.extracted_pattern_expand = { NULL, NULL, NULL, (void *) 1 },
		.filtered_files_expand = { NULL },
		.extracted_env_variable_expand = {
			&(t_vref) { NULL, 0 },
			&(t_vref) { NULL, 0 },
			&(t_vref) { NULL, 0 },
			NULL
		},
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
			},
			(void *) 1
		},
		.filtered_files_expand = { NULL },
		.extracted_env_variable_expand = {
			&(t_vref) { NULL, 0 },
			&(t_vref) { NULL, 0 },
			NULL
		},
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
			},
			(void *) 1
		},
		.filtered_files_expand = {
			&(t_list){
				.content = "a.txt",
				.next = &(t_list){
					.content = "b.txt",
					.next = NULL
				}
			}
		},
		.extracted_env_variable_expand = {
			&(t_vref) { NULL, 0 },
			&(t_vref) { NULL, 0 },
			NULL
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
			},
			(void *) 1
		},
		.filtered_files_expand = {
			&(t_list){
				.content = "test1.log",
				.next = &(t_list){
					.content = "test2.log",
					.next = NULL
				}
			}
		},
		.extracted_env_variable_expand = {
			&(t_vref) { NULL, 0 },
			&(t_vref) { NULL, 0 },
			NULL
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
			(void *) 1
		},
		.filtered_files_expand = {
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
			}
		},
		.extracted_env_variable_expand = {
			&(t_vref) { NULL, 0 },
			&(t_vref) { NULL, 0 },
			&(t_vref) { NULL, 0 },
			&(t_vref) { NULL, 0 },
			NULL
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
			},
			(void *) 1
		},
		.filtered_files_expand = {
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
		.extracted_env_variable_expand = {
			&(t_vref) { NULL, 0 },
			&(t_vref) { NULL, 0 },
			NULL
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
			},
			(void *) 1
		},
		.filtered_files_expand = {
			&(t_list){
				.content = "main.c",
				.next = &(t_list){
					.content = "readme.md",
					.next = NULL
				}
			}
		},
		.extracted_env_variable_expand = {
			&(t_vref) { NULL, 0 },
			&(t_vref) { NULL, 0 },
			NULL
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
			},
			(void *) 1
		},
		.filtered_files_expand = {
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
		.extracted_env_variable_expand = {
			&(t_vref) { NULL, 0 },
			&(t_vref) { NULL, 0 },
			NULL
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
			},
			(void *) 1
		},
		.filtered_files_expand = {
			NULL
		},
		.extracted_env_variable_expand = {
			&(t_vref) { NULL, 0 },
			&(t_vref) { NULL, 0 },
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
			},
			(void *) 1
		},
		.filtered_files_expand = {
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
		.extracted_env_variable_expand = {
			&(t_vref) { NULL, 0 },
			&(t_vref) { NULL, 0 },
			NULL
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
			},
			(void *) 1
		},
		.filtered_files_expand = {
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
		.extracted_env_variable_expand = {
			&(t_vref) { NULL, 0 },
			&(t_vref) { NULL, 0 },
			NULL
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
			},
			(void *) 1
		},
		.filtered_files_expand = {
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
		.extracted_env_variable_expand = {
			&(t_vref) { NULL, 0 },
			&(t_vref) { NULL, 0 },
			NULL
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
			},
			(void *) 1
		},
		.filtered_files_expand = {
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
		.extracted_env_variable_expand = {
			&(t_vref) { NULL, 0 },
			&(t_vref) { NULL, 0 },
			NULL
		},
		.expected_tokens_expand = { "echo", "file 1.txt", "file 2.txt", "file 3.txt", NULL },
	},
	{
		.name = "expand_one_word_var",
		.input_tokens = { "echo", "hello", "$USER", NULL },
		.extracted_pattern_expand = { NULL, NULL, NULL, (void *) 1 },
		.filtered_files_expand = { NULL },
		.extracted_env_variable_expand = {
			&(t_vref) { NULL, 0 },
			&(t_vref) { NULL, 0 },
			&(t_vref) { "$USER", 0 },
			&(t_vref) { NULL, 0 },
			NULL
		},
		.expected_tokens_expand = { "echo", "hello", "alex", NULL },
	},
	{
		.name = "expand_multiples_word_var",
		.input_tokens = { "echo", "$WELCOME2", NULL },
		.extracted_pattern_expand = { NULL, NULL, NULL, (void *) 1 },
		.filtered_files_expand = { NULL },
		.extracted_env_variable_expand = {
			&(t_vref) { NULL, 0 },
			&(t_vref) { "$WELCOME2", 0 },
			&(t_vref) { NULL, 0 },
			NULL
		},
		.expected_tokens_expand = { "echo", "Hello", "World", NULL },
	},
	{
		.name = "expand_multiples_vars",
		.input_tokens = { "echo", "$USER", "$WELCOME2", NULL },
		.extracted_pattern_expand = { NULL, NULL, NULL, NULL, (void *) 1 },
		.filtered_files_expand = { NULL },
		.extracted_env_variable_expand = {
			&(t_vref) { NULL, 0 },
			&(t_vref) { "$USER", 0 },
			&(t_vref) { NULL, 0 },
			&(t_vref) { "$WELCOME2", 0 },
			&(t_vref) { NULL, 0 },
			NULL
		},
		.expected_tokens_expand = { "echo", "alex", "Hello", "World", NULL },
	},
	{
		.name = "expand_var_dquoted",
		.input_tokens = { "echo", "hello", "\"$USER\"", NULL },
		.extracted_pattern_expand = { NULL, NULL, NULL, (void *) 1 },
		.filtered_files_expand = { NULL },
		.extracted_env_variable_expand = {
			&(t_vref) { NULL, 0 },
			&(t_vref) { NULL, 0 },
			&(t_vref) { "$USER", 1 },
			&(t_vref) { NULL, 0 },
			NULL
		},
		.expected_tokens_expand = { "echo", "hello", "\"alex\"", NULL },
	},
	{
		.name = "expand_var_squoted",
		.input_tokens = { "echo", "hello", "\'$USER\'", NULL },
		.extracted_pattern_expand = { NULL, NULL, NULL, (void *) 1 },
		.filtered_files_expand = { NULL },
		.extracted_env_variable_expand = {
			&(t_vref) { NULL, 0 },
			&(t_vref) { NULL, 0 },
			&(t_vref) { NULL, 0 },
			NULL
		},
		.expected_tokens_expand = { "echo", "hello", "\'$USER\'", NULL },
	},
	{
		.name = "expand_var_content_quoted",
		.input_tokens = { "echo", "$WELCOME3", NULL },
		.extracted_pattern_expand = { NULL, NULL, NULL, (void *) 1 },
		.filtered_files_expand = { NULL },
		.extracted_env_variable_expand = {
			&(t_vref) { NULL, 0 },
			&(t_vref) { "$WELCOME3", 0 },
			&(t_vref) { NULL, 0 },
			NULL
		},
		.expected_tokens_expand = { "echo", "\"Hello", "Alex\"", NULL },
	},
	{
		.name = "expand_var_prefix",
		.input_tokens = { "echo", "user:$USER", NULL },
		.extracted_pattern_expand = { NULL, NULL, (void *) 1 },
		.filtered_files_expand = { NULL },
		.extracted_env_variable_expand = {
			&(t_vref) { NULL, 0 },
			&(t_vref) { "$USER", 5 },
			&(t_vref) { NULL, 0 },
			NULL
		},
		.expected_tokens_expand = { "echo", "user:alex", NULL },
	},
	{
		.name = "expand_empty_var",
		.input_tokens = { "echo", "hello", "$EMPTY", NULL },
		.extracted_pattern_expand = { NULL, NULL, NULL, (void *) 1 },
		.filtered_files_expand = { NULL },
		.extracted_env_variable_expand = {
			&(t_vref) { NULL, 0 },
			&(t_vref) { NULL, 0 },
			&(t_vref) { "$EMPTY", 0 },
			&(t_vref) { NULL, 0 },
			NULL
		},
		.expected_tokens_expand = { "echo", "hello", NULL },
	},
	{
		.name = "expand_no_var",
		.input_tokens = { "echo", "hello", "$NONE", NULL },
		.extracted_pattern_expand = { NULL, NULL, NULL, (void *) 1 },
		.filtered_files_expand = { NULL },
		.extracted_env_variable_expand = {
			&(t_vref) { NULL, 0 },
			&(t_vref) { NULL, 0 },
			&(t_vref) { "$NONE", 0 },
			&(t_vref) { NULL, 0 },
			NULL
		},
		.expected_tokens_expand = { "echo", "hello", NULL },
	},
	{
		.name = "expand_var_quotted_suffix",
		.input_tokens = { "echo", "$USER\"__SUFFIX__\"", NULL },
		.extracted_pattern_expand = { NULL, NULL, (void *) 1 },
		.filtered_files_expand = { NULL },
		.extracted_env_variable_expand = {
			&(t_vref) { NULL, 0 },
			&(t_vref) { "$USER", 0 },
			&(t_vref) { NULL, 0 },
			NULL
		},
		.expected_tokens_expand = { "echo", "alex\"__SUFFIX__\"", NULL },
	},
	{
		.name = "expand_empty_var_only",
		.input_tokens = { "$EMPTY", NULL },
		.extracted_pattern_expand = { NULL, (void *) 1 },
		.filtered_files_expand = { NULL },
		.extracted_env_variable_expand = {
			&(t_vref) { "$EMPTY", 0 },
			&(t_vref) { NULL, 0 },
			NULL
		},
		.expected_tokens_expand = { NULL },
	},
	{
		.name = "expand_env_and_wildcard",
		.input_tokens = { "echo", "$WELCOME2", "*", NULL },
		.extracted_env_variable_expand = {
			&(t_vref) { NULL, 0 },
			&(t_vref) { "$WELCOME2", 0 },
			&(t_vref) { NULL, 0 },
			&(t_vref) { NULL, 0 },
			NULL
		},
		.extracted_pattern_expand = {
			NULL,
			NULL,
			NULL,
			&(t_pattern) {
				.prefix = NULL,
				.infixes = NULL,
				.suffix = NULL
			},
			(void *) 1
		},
		.filtered_files_expand = {
			&(t_list){
				.content = "a.txt",
				.next = &(t_list){
					.content = "b.txt",
					.next = &(t_list){
						.content = "c.txt",
						.next = NULL
					}
				}
			}
		},
		.expected_tokens_expand = { "echo", "Hello", "World", "a.txt", "b.txt", "c.txt", NULL },
	},
	{
		.name = "expand_wildcard_from_env",
		.input_tokens = { "echo", "$WILDCARD", NULL },.extracted_env_variable_expand = {
			&(t_vref) { NULL, 0 },
			&(t_vref) { "$WILDCARD", 0 },
			&(t_vref) { NULL, 0 },
			NULL
		},
		.extracted_pattern_expand = {
			NULL,
			&(t_pattern) {
				.prefix = NULL,
				.infixes = NULL,
				.suffix = NULL
			},
			(void *) 1
		},
		.filtered_files_expand = {
			&(t_list){
				.content = "a.txt",
				.next = &(t_list){
					.content = "b.txt",
					.next = &(t_list){
						.content = "c.txt",
						.next = NULL
					}
				}
			}
		},
		.expected_tokens_expand = { "echo", "a.txt", "b.txt", "c.txt", NULL },
	},
	{
		.name = "expand_wildcards_from_env",
		.input_tokens = { "echo", "$WILDCARDS", NULL },.extracted_env_variable_expand = {
			&(t_vref) { NULL, 0 },
			&(t_vref) { "$WILDCARDS", 0 },
			&(t_vref) { NULL, 0 },
			NULL
		},
		.extracted_pattern_expand = {
			NULL,
			NULL,
			&(t_pattern) {
				.prefix = NULL,
				.infixes = NULL,
				.suffix = ".txt"
			},
			NULL,
			&(t_pattern) {
				.prefix = NULL,
				.infixes = NULL,
				.suffix = ".pdf"
			},
			NULL,
			(void *) 1
		},
		.filtered_files_expand = {
			&(t_list){
				.content = "a.txt",
				.next = &(t_list){
					.content = "b.txt",
					.next = &(t_list){
						.content = "c.txt",
						.next = NULL
					}
				}
			},
			&(t_list){
				.content = "a.pdf",
				.next = &(t_list){
					.content = "b.pdf",
					.next = &(t_list){
						.content = "c.pdf",
						.next = NULL
					}
				}
			}
		},
		.expected_tokens_expand = { "echo", "BEFORE", "a.txt", "b.txt", "c.txt", "IN", "a.pdf", "b.pdf", "c.pdf", "AFTER", NULL },
	},
	{ NULL }
};

static char *env[] = {
	"USER=alex",
	"WELCOME=Hello",
	"WELCOME2=Hello World",
	"WELCOME3=\"Hello Alex\"",
	"EMPTY=",
	"WILDCARD=*",
	"WILDCARDS=BEFORE *.txt IN *.pdf AFTER",
	"AAA=$BBB",
	"BBB=$CCC",
	"CCC=Valorant",
	NULL
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

t_error	__wrap_scan_dir(t_list **content, char *path)
{
	(void) path;
	*content = mock_type(t_list *);
	return (mock_type(t_error));
}

t_error	__wrap_extract_var(t_vref **varpos, char *str)
{
	(void) str;
	*varpos = mock_type(t_vref *);
	return (mock_type(t_error));
}

static void expand_basic_tests(void **case_name)
{
	t_test_case	*tc;
	t_list		*tested;
	t_list		*expected;
	t_error		error;
	int			equal;
	size_t		i;
	size_t		j;

	if (case_name == NULL)
		return (printf("Implementation error.\n"), assert_true(0));
	tested = NULL;
	expected = NULL;
	tc = find_case(expand_cases, *case_name);
	if (tc == NULL)
		return (printf(CASE_NOT_FOUND_MSG, (char *) *case_name), assert_true(0));
	tested = create_token_list(tc->input_tokens);
	expected = create_token_list(tc->expected_tokens_expand);
	j = 0;
	i = 0;
	while (tc->extracted_pattern_expand[i] != (void *) 1)
	{
		will_return(__wrap_extract_pattern, pattern_dup(tc->extracted_pattern_expand[i]));
		will_return(__wrap_extract_pattern, ERR_NONE);
		if (tc->extracted_pattern_expand[i] != NULL)
		{
			will_return(__wrap_scan_dir, NULL);
			will_return(__wrap_scan_dir, ERR_NONE);
			will_return(__wrap_globbing, lst_dup(tc->filtered_files_expand[j++], (void *) ft_strdup, free));
			will_return(__wrap_globbing, ERR_NONE);
		}
		i++;
	}
	i = 0;
	while (tc->extracted_env_variable_expand[i] != NULL)
	{
		will_return(__wrap_extract_var, varpos_dup(tc->extracted_env_variable_expand[i]));
		will_return(__wrap_extract_var, ERR_NONE);
		i++;
	}
	error = expand(&tested, env);
	equal = lst_equal(expected, tested, (void *) token_equal);
	if (!equal || error != ERR_NONE)
	{
		printf(FAIL_MSG, (char *) *case_name);
		printf("Expected:\t%d\t", ERR_NONE);
		print_token_list(expected);
		printf("Tested:\t\t%d\t", error);
		print_token_list(tested);
		ft_lstclear(&tested, (void *) free_token);
		ft_lstclear(&expected, (void *) free_token);
		assert_true(0);
	}
	ft_lstclear(&tested, (void *) free_token);
	ft_lstclear(&expected, (void *) free_token);
	printf(SUCCESS_MSG, (char *) *case_name);
	return(assert_true(1));
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
		cmocka_unit_test_prestate(expand_basic_tests, expand_cases[13].name),
		cmocka_unit_test_prestate(expand_basic_tests, expand_cases[14].name),
		cmocka_unit_test_prestate(expand_basic_tests, expand_cases[15].name),
		cmocka_unit_test_prestate(expand_basic_tests, expand_cases[16].name),
		cmocka_unit_test_prestate(expand_basic_tests, expand_cases[17].name),
		cmocka_unit_test_prestate(expand_basic_tests, expand_cases[18].name),
		cmocka_unit_test_prestate(expand_basic_tests, expand_cases[19].name),
		cmocka_unit_test_prestate(expand_basic_tests, expand_cases[20].name),
		cmocka_unit_test_prestate(expand_basic_tests, expand_cases[21].name),
		cmocka_unit_test_prestate(expand_basic_tests, expand_cases[22].name),
		cmocka_unit_test_prestate(expand_basic_tests, expand_cases[23].name),
		cmocka_unit_test_prestate(expand_basic_tests, expand_cases[24].name),
		cmocka_unit_test_prestate(expand_basic_tests, expand_cases[25].name),
		cmocka_unit_test_prestate(expand_basic_tests, expand_cases[26].name),
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
