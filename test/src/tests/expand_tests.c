/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tests.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 19:39:16 by apierret          #+#    #+#             */
/*   Updated: 2025/06/05 23:07:06 by apierret         ###   ########.fr       */
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
		"no_expand",
		&(t_expand_case)
		{
			.input_tokens = {"echo", "hello", "world", NULL},
			.extracted_patterns = {NULL, NULL, NULL, (void *) 1},
			.filtered_files = {NULL},
			.extracted_vrefs = {
				&(t_vref){NULL, 0},
				&(t_vref){NULL, 0},
				&(t_vref){NULL, 0},
				NULL
			},
			.expected_tokens = {"echo", "hello", "world", NULL},
		}
	},
	{
		"no_match",
		&(t_expand_case)
		{
			.input_tokens = {"cat", "*.md", NULL},
			.extracted_patterns = {
				NULL,
				&(t_pattern){
					.prefix = NULL,
					.infixes = NULL,
					.suffix = ".md"
				},
				(void *) 1
			},
			.filtered_files = {NULL},
			.extracted_vrefs = {
				&(t_vref){NULL, 0},
				&(t_vref){NULL, 0},
				NULL
			},
			.expected_tokens = {"cat", "*.md", NULL},
		}
	},
	{
		"simple_match",
		&(t_expand_case)
		{
			.input_tokens = {"ls", "*.txt", NULL},
			.extracted_patterns = {
				NULL,
				&(t_pattern){
					.prefix = NULL,
					.infixes = NULL,
					.suffix = ".txt"
				},
				(void *) 1
			},
			.filtered_files = {
				&(t_list){
					.content = "a.txt",
					.next = &(t_list){
						.content = "b.txt",
						.next = NULL
					}
				}
			},
			.extracted_vrefs = {
				&(t_vref){NULL, 0},
				&(t_vref){NULL, 0},
				NULL
			},
			.expected_tokens = {"ls", "a.txt", "b.txt", NULL},
		}
	},
	{
		"wildcard_middle",
		&(t_expand_case)
		{
			.input_tokens = {"rm", "test*.log", NULL},
			.extracted_patterns = {
				NULL,
				&(t_pattern){
					.prefix = "test",
					.infixes = NULL,
					.suffix = ".log"
				},
				(void *) 1
			},
			.filtered_files = {
				&(t_list){
					.content = "test1.log",
					.next = &(t_list){
						.content = "test2.log",
						.next = NULL
					}
				}
			},
			.extracted_vrefs = {
				&(t_vref){NULL, 0},
				&(t_vref){NULL, 0},
				NULL
			},
			.expected_tokens = {"rm", "test1.log", "test2.log", NULL},
		}
	},
	{
		"multiple_wildcards",
		&(t_expand_case)
		{
			.input_tokens = {"cp", "*.c", "*.h", "backup/", NULL},
			.extracted_patterns = {
				NULL,
				&(t_pattern){
					.prefix = NULL,
					.infixes = NULL,
					.suffix = ".c"
				},
				&(t_pattern){
					.prefix = NULL,
					.infixes = NULL,
					.suffix = ".h"
				},
				NULL,
				(void *) 1
			},
			.filtered_files = {
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
			.extracted_vrefs = {
				&(t_vref){NULL, 0},
				&(t_vref){NULL, 0},
				&(t_vref){NULL, 0},
				&(t_vref){NULL, 0},
				NULL
			},
			.expected_tokens = {
				"cp", "main.c", "utils.c", "utils.h", "backup/", NULL
			},
		}
	},
	{
		"match_all_files",
		&(t_expand_case)
		{
			.input_tokens = {"echo", "*", NULL},
			.extracted_patterns = {
				NULL,
				&(t_pattern){
					.prefix = NULL,
					.infixes = NULL,
					.suffix = NULL
				},
				(void *) 1
			},
			.filtered_files = {
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
			.extracted_vrefs = {
				&(t_vref){NULL, 0},
				&(t_vref){NULL, 0},
				NULL
			},
			.expected_tokens = {"echo", "a.txt", "b.md", "c.pdf", NULL},
		}
	},
	{
		"wildcard_with_dot",
		&(t_expand_case)
		{
			.input_tokens = {"cat", "*.*", NULL},
			.extracted_patterns = {
				NULL,
				&(t_pattern){
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
			.filtered_files = {
				&(t_list){
					.content = "main.c",
					.next = &(t_list){
						.content = "readme.md",
						.next = NULL
					}
				}
			},
			.extracted_vrefs = {
				&(t_vref){NULL, 0},
				&(t_vref){NULL, 0},
				NULL
			},
			.expected_tokens = {"cat", "main.c", "readme.md", NULL},
		}
	},
	{
		"hidden_files",
		&(t_expand_case)
		{
			.input_tokens = {"echo", ".*", NULL},
			.extracted_patterns = {
				NULL,
				&(t_pattern){
					.prefix = ".",
					.infixes = NULL,
					.suffix = NULL
				},
				(void *) 1
			},
			.filtered_files = {
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
			.extracted_vrefs = {
				&(t_vref){NULL, 0},
				&(t_vref){NULL, 0},
				NULL
			},
			.expected_tokens = {
				"echo", ".bash_history", ".cache", ".config", NULL
			},
		}
	},
	{
		"empty_directory",
		&(t_expand_case)
		{
			.input_tokens = {"echo", "*", NULL},
			.extracted_patterns = {
				NULL,
				&(t_pattern){
					.prefix = NULL,
					.infixes = NULL,
					.suffix = NULL
				},
				(void *) 1
			},
			.filtered_files = {
				NULL
			},
			.extracted_vrefs = {
				&(t_vref){NULL, 0},
				&(t_vref){NULL, 0},
				NULL
			},
			.expected_tokens = {"echo", "*", NULL},
		}
	},
	{
		"prefix_only",
		&(t_expand_case)
		{
			.input_tokens = {"echo", "main*", NULL},
			.extracted_patterns = {
				NULL,
				&(t_pattern){
					.prefix = "main",
					.infixes = NULL,
					.suffix = NULL
				},
				(void *) 1
			},
			.filtered_files = {
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
			.extracted_vrefs = {
				&(t_vref){NULL, 0},
				&(t_vref){NULL, 0},
				NULL
			},
			.expected_tokens = {
				"echo", "main.c", "main.h", "main_backup.c", NULL
			},
		}
	},
	{
		"suffix_only",
		&(t_expand_case)
		{
			.input_tokens = {"echo", "*.c", NULL},
			.extracted_patterns = {
				NULL,
				&(t_pattern){
					.prefix = NULL,
					.infixes = NULL,
					.suffix = ".c"
				},
				(void *) 1
			},
			.filtered_files = {
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
			.extracted_vrefs = {
				&(t_vref){NULL, 0},
				&(t_vref){NULL, 0},
				NULL
			},
			.expected_tokens = {
				"echo", "main.c", "tests.c", "main_backup.c", NULL
			},
		}
	},
	{
		"literal_asterisk",
		&(t_expand_case)
		{
			.input_tokens = {"echo", "file*.txt", NULL},
			.extracted_patterns = {
				NULL,
				&(t_pattern){
					.prefix = "file",
					.infixes = NULL,
					.suffix = ".txt"
				},
				(void *) 1
			},
			.filtered_files = {
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
			.extracted_vrefs = {
				&(t_vref){NULL, 0},
				&(t_vref){NULL, 0},
				NULL
			},
			.expected_tokens = {
				"echo", "file*.txt", "file1.txt", "file2.txt", NULL
			},
		}
	},
	{
		"file_with_space",
		&(t_expand_case)
		{
			.input_tokens = {"echo", "file*.txt", NULL},
			.extracted_patterns = {
				NULL,
				&(t_pattern){
					.prefix = "file",
					.infixes = NULL,
					.suffix = ".txt"
				},
				(void *) 1
			},
			.filtered_files = {
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
			.extracted_vrefs = {
				&(t_vref){NULL, 0},
				&(t_vref){NULL, 0},
				NULL
			},
			.expected_tokens = {
				"echo", "file 1.txt", "file 2.txt", "file 3.txt", NULL
			},
		}
	},
	{
		"expand_one_word_var",
		&(t_expand_case)
		{
			.input_tokens = {"echo", "hello", "$USER", NULL},
			.extracted_patterns = {NULL, NULL, NULL, (void *) 1},
			.filtered_files = {NULL},
			.extracted_vrefs = {
				&(t_vref){NULL, 0},
				&(t_vref){NULL, 0},
				&(t_vref){"$USER", 0},
				&(t_vref){NULL, 0},
				NULL
			},
			.expected_tokens = {"echo", "hello", "alex", NULL},
		}
	},
	{
		"expand_multiples_word_var",
		&(t_expand_case)
		{
			.input_tokens = {"echo", "$WELCOME2", NULL},
			.extracted_patterns = {NULL, NULL, NULL, (void *) 1},
			.filtered_files = {NULL},
			.extracted_vrefs = {
				&(t_vref){NULL, 0},
				&(t_vref){"$WELCOME2", 0},
				&(t_vref){NULL, 0},
				NULL
			},
			.expected_tokens = {"echo", "Hello", "World", NULL},
		}
	},
	{
		"expand_multiples_vars",
		&(t_expand_case)
		{
			.input_tokens = {"echo", "$USER", "$WELCOME2", NULL},
			.extracted_patterns = {NULL, NULL, NULL, NULL, (void *) 1},
			.filtered_files = {NULL},
			.extracted_vrefs = {
				&(t_vref){NULL, 0},
				&(t_vref){"$USER", 0},
				&(t_vref){NULL, 0},
				&(t_vref){"$WELCOME2", 0},
				&(t_vref){NULL, 0},
				NULL
			},
			.expected_tokens = {"echo", "alex", "Hello", "World", NULL},
		}
	},
	{
		"expand_var_dquoted",
		&(t_expand_case)
		{
			.input_tokens = {"echo", "hello", "\"$USER\"", NULL},
			.extracted_patterns = {NULL, NULL, NULL, (void *) 1},
			.filtered_files = {NULL},
			.extracted_vrefs = {
				&(t_vref){NULL, 0},
				&(t_vref){NULL, 0},
				&(t_vref){"$USER", 1},
				&(t_vref){NULL, 0},
				NULL
			},
			.expected_tokens = {"echo", "hello", "\"alex\"", NULL},
		}
	},
	{
		"expand_var_squoted",
		&(t_expand_case)
		{
			.input_tokens = {"echo", "hello", "\'$USER\'", NULL},
			.extracted_patterns = {NULL, NULL, NULL, (void *) 1},
			.filtered_files = {NULL},
			.extracted_vrefs = {
				&(t_vref){NULL, 0},
				&(t_vref){NULL, 0},
				&(t_vref){NULL, 0},
				NULL
			},
			.expected_tokens = {"echo", "hello", "\'$USER\'", NULL},
		}
	},
	{
		"expand_var_content_quoted",
		&(t_expand_case)
		{
			.input_tokens = {"echo", "$WELCOME3", NULL},
			.extracted_patterns = {NULL, NULL, NULL, (void *) 1},
			.filtered_files = {NULL},
			.extracted_vrefs = {
				&(t_vref){NULL, 0},
				&(t_vref){"$WELCOME3", 0},
				&(t_vref){NULL, 0},
				NULL
			},
			.expected_tokens = {"echo", "\"Hello", "Alex\"", NULL},
		}
	},
	{
		"expand_var_prefix",
		&(t_expand_case)
		{
			.input_tokens = {"echo", "user:$USER", NULL},
			.extracted_patterns = {NULL, NULL, (void *) 1},
			.filtered_files = {NULL},
			.extracted_vrefs = {
				&(t_vref){NULL, 0},
				&(t_vref){"$USER", 5},
				&(t_vref){NULL, 0},
				NULL
			},
			.expected_tokens = {"echo", "user:alex", NULL},
		}
	},
	{
		"expand_empty_var",
		&(t_expand_case)
		{
			.input_tokens = {"echo", "hello", "$EMPTY", NULL},
			.extracted_patterns = {NULL, NULL, NULL, (void *) 1},
			.filtered_files = {NULL},
			.extracted_vrefs = {
				&(t_vref){NULL, 0},
				&(t_vref){NULL, 0},
				&(t_vref){"$EMPTY", 0},
				&(t_vref){NULL, 0},
				NULL
			},
			.expected_tokens = {"echo", "hello", NULL},
		}
	},
	{
		"expand_no_var",
		&(t_expand_case)
		{
			.input_tokens = {"echo", "hello", "$NONE", NULL},
			.extracted_patterns = {NULL, NULL, NULL, (void *) 1},
			.filtered_files = {NULL},
			.extracted_vrefs = {
				&(t_vref){NULL, 0},
				&(t_vref){NULL, 0},
				&(t_vref){"$NONE", 0},
				&(t_vref){NULL, 0},
				NULL
			},
			.expected_tokens = {"echo", "hello", NULL},
		}
	},
	{
		"expand_var_quotted_suffix",
		&(t_expand_case)
		{
			.input_tokens = {"echo", "$USER\"__SUFFIX__\"", NULL},
			.extracted_patterns = {NULL, NULL, (void *) 1},
			.filtered_files = {NULL},
			.extracted_vrefs = {
				&(t_vref){NULL, 0},
				&(t_vref){"$USER", 0},
				&(t_vref){NULL, 0},
				NULL
			},
			.expected_tokens = {"echo", "alex\"__SUFFIX__\"", NULL},
		}
	},
	{
		"expand_empty_var_only",
		&(t_expand_case)
		{
			.input_tokens = {"$EMPTY", NULL},
			.extracted_patterns = {NULL, (void *) 1},
			.filtered_files = {NULL},
			.extracted_vrefs = {
				&(t_vref){"$EMPTY", 0},
				&(t_vref){NULL, 0},
				NULL
			},
			.expected_tokens = {NULL},
		}
	},
	{
		"expand_env_and_wildcard",
		&(t_expand_case)
		{
			.input_tokens = {"echo", "$WELCOME2", "*", NULL},
			.extracted_vrefs = {
				&(t_vref){NULL, 0},
				&(t_vref){"$WELCOME2", 0},
				&(t_vref){NULL, 0},
				&(t_vref){NULL, 0},
				NULL
			},
			.extracted_patterns = {
				NULL,
				NULL,
				NULL,
				&(t_pattern){
					.prefix = NULL,
					.infixes = NULL,
					.suffix = NULL
				},
				(void *) 1
			},
			.filtered_files = {
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
			.expected_tokens = {
				"echo", "Hello", "World", "a.txt", "b.txt", "c.txt", NULL
			},
		}
	},
	{
		"expand_wildcard_from_env",
		&(t_expand_case)
		{
			.input_tokens = {"echo", "$WILDCARD", NULL},
			.extracted_vrefs = {
				&(t_vref){NULL, 0},
				&(t_vref){"$WILDCARD", 0},
				&(t_vref){NULL, 0},
				NULL
			},
			.extracted_patterns = {
				NULL,
				&(t_pattern){
					.prefix = NULL,
					.infixes = NULL,
					.suffix = NULL
				},
				(void *) 1
			},
			.filtered_files = {
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
			.expected_tokens = {"echo", "a.txt", "b.txt", "c.txt", NULL},
		}
	},
	{
		"expand_wildcards_from_env",
		&(t_expand_case)
		{
			.input_tokens = {"echo", "$WILDCARDS", NULL},
			.extracted_vrefs = {
				&(t_vref){NULL, 0},
				&(t_vref){"$WILDCARDS", 0},
				&(t_vref){NULL, 0},
				NULL
			},
			.extracted_patterns = {
				NULL,
				NULL,
				&(t_pattern){
					.prefix = NULL,
					.infixes = NULL,
					.suffix = ".txt"
				},
				NULL,
				&(t_pattern){
					.prefix = NULL,
					.infixes = NULL,
					.suffix = ".pdf"
				},
				NULL,
				(void *) 1
			},
			.filtered_files = {
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
			.expected_tokens = {
				"echo", "BEFORE", "a.txt", "b.txt", "c.txt", "IN", "a.pdf",
				"b.pdf", "c.pdf", "AFTER", NULL
			},
		}
	},
	{NULL}
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
static t_list	*tested;
static t_list	*expected;

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

static int	test_setup(void **state) {
	(void) state;
	tested = NULL;
	expected = NULL;
	return (0);
}

static int	test_teardown(void **state) {
	(void) state;
	ft_lstclear(&tested, (void *) free_token);
	ft_lstclear(&expected, (void *) free_token);
	return (0);
}

static void expand_basic_tests(void **case_name)
{
	t_test_case			*tc;
	t_expand_case		*data;
	size_t				i;
	size_t				j;
	t_error				error;
	int					equal;

	if (case_name == NULL)
		return(printf("Implementation error.\n"), assert_true(0));
	tc = find_case(expand_cases, *case_name);
	if (tc == NULL)
		return (printf(CASE_NOT_FOUND_MSG, (char *) *case_name), assert_true(0));
	data = tc->data;
	expected = create_token_list(data->expected_tokens);
	if (expected == NULL && data->expected_tokens[0] != NULL)
		return(printf(FAIL_MSG, (char *) *case_name, "create_token_list failed"), assert_true(0));
	tested = create_token_list(data->input_tokens);
	if (tested == NULL && data->input_tokens[0] != NULL)
		return(printf(FAIL_MSG, (char *) *case_name, "create_token_list failed"), assert_true(0));
	i = 0;
	j = 0;
	while (data->extracted_patterns[i] != (void *) 1)
	{
		will_return(__wrap_extract_pattern, pattern_dup(data->extracted_patterns[i]));
		will_return(__wrap_extract_pattern, ERR_NONE);
		if (data->extracted_patterns[i] != NULL)
		{
			will_return(__wrap_scan_dir, NULL);
			will_return(__wrap_scan_dir, ERR_NONE);
			will_return(__wrap_globbing, lst_dup(data->filtered_files[j++], (void *) ft_strdup, free));
			will_return(__wrap_globbing, ERR_NONE);
		}
		i++;
	}
	i = 0;
	while (data->extracted_vrefs[i] != NULL)
	{
		will_return(__wrap_extract_var, varpos_dup(data->extracted_vrefs[i]));
		will_return(__wrap_extract_var, ERR_NONE);
		i++;
	}
	error = expand(&tested, env);
	equal = lst_equal(expected, tested, (void *) token_equal);
	if (!equal || error != ERR_NONE)
	{
		printf(FAIL_MSG, (char *) *case_name, "");
		printf("Expected:\t%d\t", ERR_NONE);
		print_token_list(expected);
		printf("Tested:\t\t%d\t", error);
		print_token_list(tested);
		assert_true(0);
	}
	if (error != ERR_NONE)
		return(printf(FAIL_MSG, (char *) *case_name, "error"), assert_true(0));
	if (!equal)
		return(printf(FAIL_MSG, (char *) *case_name, "not equal"), assert_true(0));
	return (printf(SUCCESS_MSG, (char *) *case_name), assert_true(1));
}

t_test_result	execute_tests(void)
{
	const struct CMUnitTest test_cases[] = {
		cmocka_unit_test_prestate_setup_teardown(expand_basic_tests, test_setup, test_teardown, expand_cases[0].name),
		cmocka_unit_test_prestate_setup_teardown(expand_basic_tests, test_setup, test_teardown, expand_cases[1].name),
		cmocka_unit_test_prestate_setup_teardown(expand_basic_tests, test_setup, test_teardown, expand_cases[2].name),
		cmocka_unit_test_prestate_setup_teardown(expand_basic_tests, test_setup, test_teardown, expand_cases[3].name),
		cmocka_unit_test_prestate_setup_teardown(expand_basic_tests, test_setup, test_teardown, expand_cases[4].name),
		cmocka_unit_test_prestate_setup_teardown(expand_basic_tests, test_setup, test_teardown, expand_cases[5].name),
		cmocka_unit_test_prestate_setup_teardown(expand_basic_tests, test_setup, test_teardown, expand_cases[6].name),
		cmocka_unit_test_prestate_setup_teardown(expand_basic_tests, test_setup, test_teardown, expand_cases[7].name),
		cmocka_unit_test_prestate_setup_teardown(expand_basic_tests, test_setup, test_teardown, expand_cases[8].name),
		cmocka_unit_test_prestate_setup_teardown(expand_basic_tests, test_setup, test_teardown, expand_cases[9].name),
		cmocka_unit_test_prestate_setup_teardown(expand_basic_tests, test_setup, test_teardown, expand_cases[10].name),
		cmocka_unit_test_prestate_setup_teardown(expand_basic_tests, test_setup, test_teardown, expand_cases[11].name),
		cmocka_unit_test_prestate_setup_teardown(expand_basic_tests, test_setup, test_teardown, expand_cases[12].name),
		cmocka_unit_test_prestate_setup_teardown(expand_basic_tests, test_setup, test_teardown, expand_cases[13].name),
		cmocka_unit_test_prestate_setup_teardown(expand_basic_tests, test_setup, test_teardown, expand_cases[14].name),
		cmocka_unit_test_prestate_setup_teardown(expand_basic_tests, test_setup, test_teardown, expand_cases[15].name),
		cmocka_unit_test_prestate_setup_teardown(expand_basic_tests, test_setup, test_teardown, expand_cases[16].name),
		cmocka_unit_test_prestate_setup_teardown(expand_basic_tests, test_setup, test_teardown, expand_cases[17].name),
		cmocka_unit_test_prestate_setup_teardown(expand_basic_tests, test_setup, test_teardown, expand_cases[18].name),
		cmocka_unit_test_prestate_setup_teardown(expand_basic_tests, test_setup, test_teardown, expand_cases[19].name),
		cmocka_unit_test_prestate_setup_teardown(expand_basic_tests, test_setup, test_teardown, expand_cases[20].name),
		cmocka_unit_test_prestate_setup_teardown(expand_basic_tests, test_setup, test_teardown, expand_cases[21].name),
		cmocka_unit_test_prestate_setup_teardown(expand_basic_tests, test_setup, test_teardown, expand_cases[22].name),
		cmocka_unit_test_prestate_setup_teardown(expand_basic_tests, test_setup, test_teardown, expand_cases[23].name),
		cmocka_unit_test_prestate_setup_teardown(expand_basic_tests, test_setup, test_teardown, expand_cases[24].name),
		cmocka_unit_test_prestate_setup_teardown(expand_basic_tests, test_setup, test_teardown, expand_cases[25].name),
		cmocka_unit_test_prestate_setup_teardown(expand_basic_tests, test_setup, test_teardown, expand_cases[26].name),
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
