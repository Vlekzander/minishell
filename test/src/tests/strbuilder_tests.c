/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strbuilder_tests.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by apierret          #+#    #+#             */
/*   Updated: 2025/06/09 22:39:12 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "test.h"
#include "test_utils.h"

static t_test_case strbuilder_cases[] =
{
	{
		"simple_helloworld",
		&(t_strbuilder_case){
	            {
	            	"hello world",
					NULL
				},
				"hello world"
			}
	},
	{
		"splitted_helloworld",
		&(t_strbuilder_case){
	            {
	            	"hello",
					" ",
					"world",
					NULL
				},
				"hello world"
			}
	},
	{
		"simple_empty",
		&(t_strbuilder_case){
	            {
	            	"",
					NULL
				},
				""
			}
	},
	{
		"mult_empty",
		&(t_strbuilder_case){
	            {
	            	"",
					"",
					"",
					"",
					NULL
				},
				""
			}
	},
	{
		"mutliples_long_strs",
		&(t_strbuilder_case){
			            {
		            		"I have been underwater\n",
							"It has always made me wonder\n",
							"Why it felt like all my skin was on fire\n",
							"Thinking about what is better\n",
							"Should I swim, or let it burn me\n",
							"While I'm sinking deep into my own lies?\n",
							NULL
						},
						"I have been underwater\nIt has always made me wonder\nWhy it felt like all my skin was on fire\nThinking about what is better\nShould I swim, or let it burn me\nWhile I'm sinking deep into my own lies?\n"
					}
	},
	{NULL}
};

t_strbuilder *sb;

static int	test_setup(void **state)
{
	(void) state;
	sb = create_strbuilder(16);
	if (sb == NULL)
		return (-1);
	return (0);
}

static int	test_teardown(void **state) {
	(void) state;
	free_strbuilder(sb);
	return (0);
}

static void strbuilder_tests(void **case_name)
{
	t_test_case			*tc;
	t_strbuilder_case	*data;
	int					equal;

	if (case_name == NULL)
		return(printf("Implementation error.\n"), assert_true(0));
	tc = find_case(strbuilder_cases, *case_name);
	if (tc == NULL)
		return (printf(CASE_NOT_FOUND_MSG, (char *) *case_name), assert_true(0));
	data = tc->data;
	for (int i = 0; data->strs[i] != NULL; i++)
		strbuilder_append(sb, data->strs[i]);
	equal = str_equal(sb->buffer, data->str);
	if (!equal)
		return(printf(FAIL_MSG, (char *) *case_name, "not equal"), assert_true(0));
	return (printf(SUCCESS_MSG, (char *) *case_name), assert_true(1));
}

t_test_result	execute_tests(void)
{
	const struct CMUnitTest test_cases[] = {
		cmocka_unit_test_prestate_setup_teardown(strbuilder_tests, test_setup, test_teardown, strbuilder_cases[0].name),
		cmocka_unit_test_prestate_setup_teardown(strbuilder_tests, test_setup, test_teardown, strbuilder_cases[1].name),
		cmocka_unit_test_prestate_setup_teardown(strbuilder_tests, test_setup, test_teardown, strbuilder_cases[2].name),
		cmocka_unit_test_prestate_setup_teardown(strbuilder_tests, test_setup, test_teardown, strbuilder_cases[3].name),
		cmocka_unit_test_prestate_setup_teardown(strbuilder_tests, test_setup, test_teardown, strbuilder_cases[4].name),
	};
	char	name[] = "data/string_builder";
	t_test_result	result;

	printf(GROUP_HEADER, name);
	result.total = sizeof(test_cases)/sizeof(struct CMUnitTest);
	result.failed = cmocka_run_group_tests_name(name, test_cases, NULL, NULL);
	result.successful = result.total - result.failed;
	printf(GROUP_RESULT, name, result.successful, result.total);
	return (result);
}