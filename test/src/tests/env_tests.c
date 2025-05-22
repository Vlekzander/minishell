/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_tests.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:53:22 by apierret          #+#    #+#             */
/*   Updated: 2025/05/22 19:03:19 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "env.h"
#include "test.h"
#include "test_utils.h"
#include "utils.h"

static t_test_case env_cases[] =
{
	 {
		.name = "simple_env",
		.expected_env = {
			"KEY1=VALUE1",
			"KEY2=VALUE2",
			"KEY3=VALUE3",
			NULL
		}
	 },
	{
		.name = "another_simple_env",
		.expected_env = {
			"AAA=111",
			"BBB=222",
			"CCC=333",
			"DDD=444",
			"EEE=555",
			"kwa=feur",
			NULL
		}
	},
	{
		.name = "empty_env",
		.expected_env = {
			NULL
		}
	},
	{ NULL }
};

static void load_env_basic_tests(void **case_name)
{
	t_test_case	*tc;
	char		**expected;
	char		**tested;
	t_error		error;
	int			equal;

	if (case_name == NULL)
		return (printf("Implementation error.\n"), assert_true(0));
	tc = find_case(env_cases, *case_name);
	if (tc == NULL)
		return (printf(CASE_NOT_FOUND_MSG, (char *) *case_name), assert_true(0));
	tested = NULL;
	expected = tc->expected_env;
	error = load_env(&tested, tc->expected_env);
	equal = str_array_equal(expected, tested);
	free_ddarray((void **) tested);
	if (!equal || error != ERR_NONE)
		return (printf(FAIL_MSG, (char *) *case_name), assert_true(0));
	return (printf(SUCCESS_MSG, (char *) *case_name), assert_true(1));
}

t_test_result	execute_tests(void)
{
	const struct CMUnitTest test_cases[] = {
		cmocka_unit_test_prestate(load_env_basic_tests, env_cases[0].name),
		cmocka_unit_test_prestate(load_env_basic_tests, env_cases[1].name),
		cmocka_unit_test_prestate(load_env_basic_tests, env_cases[2].name),
	};
	char			name[] = "env/CRUD";
	t_test_result	result;

	printf(GROUP_HEADER, name);
	result.total = sizeof(test_cases)/sizeof(struct CMUnitTest);
	result.failed = cmocka_run_group_tests_name(name, test_cases, NULL, NULL);
	result.successful = result.total - result.failed;
	printf(GROUP_RESULT, name, result.successful, result.total);
	return (result);
}
