/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_tests.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:25:13 by apierret          #+#    #+#             */
/*   Updated: 2025/06/05 17:06:45 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "env.h"
#include "test.h"
#include "test_utils.h"
#include "utils.h"

static t_test_case env_cases[] =
{
	{
		"load_simple_env",
		&(t_env_case)
		{
			.base_env = {
				"KEY1=VALUE1",
				"KEY2=VALUE2",
				"KEY3=VALUE3",
				NULL
			}
		}
	},
	{
		"load_another_simple_env",
		&(t_env_case)
		{
			.base_env = {
				"AAA=111",
				"BBB=222",
				"CCC=333",
				"DDD=444",
				"EEE=555",
				"kwa=feur",
				NULL
			}
		}
	},
	{
		"load_empty_env",
		&(t_env_case)
		{
			.base_env = {
				NULL
			}
		}
	},
	{
		"get_simple_env",
		&(t_env_case)
		{
			.base_env = {
				"KEY1=VALUE1",
				"KEY2=VALUE2",
				"KEY3=VALUE3",
				NULL
			},
			.str_env = "KEY1=VALUE1\nKEY2=VALUE2\nKEY3=VALUE3\n"
		}
	},
	{
		"get_another_simple_env",
		&(t_env_case)
		{
			.base_env = {
				"AAA=111",
				"BBB=222",
				"CCC=333",
				"DDD=444",
				"EEE=555",
				"kwa=feur",
				NULL
			},
			.str_env = "AAA=111\nBBB=222\nCCC=333\nDDD=444\nEEE=555\nkwa=feur\n"
		}
	},
	{
		"get_empty_env",
		&(t_env_case)
		{
			.base_env = {
				NULL
			},
			.str_env = ""
		}
	},
	{
		"get_var_first",
		&(t_env_case)
		{
			.base_env = {
				"AAA=111",
				"BBB=222",
				"CCC=333",
				"DDD=444",
				"EEE=555",
				"kwa=feur",
				NULL
			},
			.key_env = "AAA",
			.value_env = "111"
		}
	},
	{
		"get_var_last",
		&(t_env_case)
		{
			.base_env = {
				"AAA=111",
				"BBB=222",
				"CCC=333",
				"DDD=444",
				"EEE=555",
				"kwa=feur",
				NULL
			},
			.key_env = "kwa",
			.value_env = "feur"
		}
	},
	{
		"get_var_middle",
		&(t_env_case)
		{
			.base_env = {
				"AAA=111",
				"BBB=222",
				"CCC=333",
				"DDD=444",
				"EEE=555",
				"kwa=feur",
				NULL
			},
			.key_env = "CCC",
			.value_env = "333"
		}
	},
	{
		"get_var_invalid",
		&(t_env_case)
		{
			.base_env = {
				"AAA=111",
				"BBB=222",
				"CCC=333",
				"DDD=444",
				"EEE=555",
				"kwa=feur",
				NULL
			},
			.key_env = "ZZZ",
			.value_env = NULL
		}
	},
	{
		"get_var_empty",
		&(t_env_case)
		{
			.base_env = {
				"AAA=111",
				"BBB=222",
				"CCC=333",
				"DDD=444",
				"EEE=555",
				"ZZZ=",
				NULL
			},
			.key_env = "ZZZ",
			.value_env = ""
		}
	},
	{
		"remove_var_first",
		&(t_env_case)
		{
			.base_env = {
				"AAA=111",
				"BBB=222",
				"CCC=333",
				"DDD=444",
				"EEE=555",
				"kwa=feur",
				NULL
			},
			.key_env = "AAA",
			.edited_env = {
				"BBB=222",
				"CCC=333",
				"DDD=444",
				"EEE=555",
				"kwa=feur",
				NULL
			}
		}
	},
	{
		"remove_var_last",
		&(t_env_case)
		{
			.base_env = {
				"AAA=111",
				"BBB=222",
				"CCC=333",
				"DDD=444",
				"EEE=555",
				"kwa=feur",
				NULL
			},
			.key_env = "kwa",
			.edited_env = {
				"AAA=111",
				"BBB=222",
				"CCC=333",
				"DDD=444",
				"EEE=555",
				NULL
			}
		}
	},
	{
		"remove_var_middle",
		&(t_env_case)
		{
			.base_env = {
				"AAA=111",
				"BBB=222",
				"CCC=333",
				"DDD=444",
				"EEE=555",
				"kwa=feur",
				NULL
			},
			.key_env = "CCC",
			.edited_env = {
				"AAA=111",
				"BBB=222",
				"DDD=444",
				"EEE=555",
				"kwa=feur",
				NULL
			}
		}
	},
	{
		"remove_var_invalid",
		&(t_env_case)
		{
			.base_env = {
				"AAA=111",
				"BBB=222",
				"CCC=333",
				"DDD=444",
				"EEE=555",
				"kwa=feur",
				NULL
			},
			.key_env = "ZZZ",
			.edited_env = {
				"AAA=111",
				"BBB=222",
				"CCC=333",
				"DDD=444",
				"EEE=555",
				"kwa=feur",
				NULL
			}
		}
	},
	{
		"remove_var_empty",
		&(t_env_case)
		{
			.base_env = {
				NULL
			},
			.key_env = "ZZZ",
			.edited_env = {
				NULL
			}
		}
	},
{
	"set_var_new",
	&(t_env_case)
	{
		.base_env = {
			"AAA=111",
			"BBB=222",
			"CCC=333",
			NULL
		},
		.key_env = "DDD",
		.value_env = "444",
		.edited_env = {
			"AAA=111",
			"BBB=222",
			"CCC=333",
			"DDD=444",
			NULL
		}
	}
},
{
	"set_var_exist",
	&(t_env_case)
	{
		.base_env = {
			"AAA=111",
			"BBB=222",
			"CCC=333",
			NULL
		},
		.key_env = "CCC",
		.value_env = "___",
		.edited_env = {
			"AAA=111",
			"BBB=222",
			"CCC=___",
			NULL
		}
	}
},
{
	"set_var_empty_value",
	&(t_env_case)
	{
		.base_env = {
			"AAA=111",
			"BBB=222",
			"CCC=333",
			NULL
		},
		.key_env = "CCC",
		.value_env = "",
		.edited_env = {
			"AAA=111",
			"BBB=222",
			"CCC=",
			NULL
		}
	}
},
{
	"set_var_new_empty_env",
	&(t_env_case)
	{
		.base_env = {
			NULL
		},
		.key_env = "kwa",
		.value_env = "feur",
		.edited_env = {
			"kwa=feur",
			NULL
		}
	}
},
{
	"set_var_exist_alone",
	&(t_env_case)
	{
		.base_env = {
			"kwa=feur",
			NULL
		},
		.key_env = "kwa",
		.value_env = "kou",
		.edited_env = {
			"kwa=kou",
			NULL
		}
	}
},
	{ NULL }
};

char	**env_expected;
char	**env_tested;
char	*get_expected;
char	*get_env_tested;
char	*get_var_tested;

static int test_setup(void **state) {
	(void) state;
	env_expected = NULL;
	env_tested = NULL;
	get_expected = NULL;
	get_env_tested = NULL;
	get_var_tested = NULL;
	return (0);
}

static int test_teardown(void **state) {
	(void) state;
	free_ddarray((void **) env_tested);
	free(get_env_tested);
	return (0);
}

static void	load_env_basic_tests(void **case_name)
{
	t_test_case	*tc;
	t_env_case	*data;
	t_error		error;
	int			equal;

	if (case_name == NULL)
		return(printf("Implementation error.\n"), assert_true(0));
	tc = find_case(env_cases, *case_name);
	if (tc == NULL)
		return (printf(CASE_NOT_FOUND_MSG, (char *) *case_name), assert_true(0));
	data = tc->data;
	env_expected = data->base_env;
	error = load_env(&env_tested, data->base_env);
	equal = str_array_equal(env_expected, env_tested);
	if (error != ERR_NONE)
		return(printf(FAIL_MSG, (char *) *case_name, "error"), assert_true(0));
	if (!equal)
		return(printf(FAIL_MSG, (char *) *case_name, "not equal"), assert_true(0));
	return (printf(SUCCESS_MSG, (char *) *case_name), assert_true(1));
}

static void	get_env_basic_tests(void **case_name)
{
	t_test_case	*tc;
	t_env_case	*data;
	t_error		error;
	int			equal;

	if (case_name == NULL)
		return(printf("Implementation error.\n"), assert_true(0));
	tc = find_case(env_cases, *case_name);
	if (tc == NULL)
		return (printf(CASE_NOT_FOUND_MSG, (char *) *case_name), assert_true(0));
	data = tc->data;
	get_expected = data->str_env;
	error = get_env(&get_env_tested, data->base_env);
	equal = str_equal(get_expected, get_env_tested);
	if (error != ERR_NONE)
		return(printf(FAIL_MSG, (char *) *case_name, "error"), assert_true(0));
	if (!equal)
		return(printf(FAIL_MSG, (char *) *case_name, "not equal"), assert_true(0));
	return (printf(SUCCESS_MSG, (char *) *case_name), assert_true(1));
}

static void	get_var_basic_tests(void **case_name)
{
	t_test_case	*tc;
	t_env_case	*data;
	t_error		error;
	int			equal;

	if (case_name == NULL)
		return(printf("Implementation error.\n"), assert_true(0));
	tc = find_case(env_cases, *case_name);
	if (tc == NULL)
		return (printf(CASE_NOT_FOUND_MSG, (char *) *case_name), assert_true(0));
	data = tc->data;
	get_expected = data->value_env;
	error = get_var(&get_var_tested, data->base_env, data->key_env);
	equal = str_equal(get_expected, get_var_tested);
	if (error != ERR_NONE)
		return(printf(FAIL_MSG, (char *) *case_name, "error"), assert_true(0));
	if (!equal)
		return(printf(FAIL_MSG, (char *) *case_name, "not equal"), assert_true(0));
	return (printf(SUCCESS_MSG, (char *) *case_name), assert_true(1));
}

static void	remove_var_basic_tests(void **case_name)
{
	t_test_case	*tc;
	t_env_case	*data;
	t_error		error;
	int			equal;

	if (case_name == NULL)
		return(printf("Implementation error.\n"), assert_true(0));
	tc = find_case(env_cases, *case_name);
	if (tc == NULL)
		return (printf(CASE_NOT_FOUND_MSG, (char *) *case_name), assert_true(0));
	data = tc->data;
	env_expected = data->edited_env;
	error = load_env(&env_tested, data->base_env);
	if (error != ERR_NONE)
		return(printf(FAIL_MSG, (char *) *case_name, "load_env failed"), assert_true(0));
	error = remove_var(&env_tested, data->key_env);
	equal = str_array_equal(env_expected, env_tested);
	if (error != ERR_NONE)
		return(printf(FAIL_MSG, (char *) *case_name, "error"), assert_true(0));
	if (!equal)
		return(printf(FAIL_MSG, (char *) *case_name, "not equal"), assert_true(0));
	return (printf(SUCCESS_MSG, (char *) *case_name), assert_true(1));
}

static void	set_var_basic_tests(void **case_name)
{
	t_test_case	*tc;
	t_env_case	*data;
	t_error		error;
	int			equal;

	if (case_name == NULL)
		return(printf("Implementation error.\n"), assert_true(0));
	tc = find_case(env_cases, *case_name);
	if (tc == NULL)
		return (printf(CASE_NOT_FOUND_MSG, (char *) *case_name), assert_true(0));
	data = tc->data;
	env_expected = data->edited_env;
	error = load_env(&env_tested, data->base_env);
	if (error != ERR_NONE)
		return(printf(FAIL_MSG, (char *) *case_name, "load_env failed"), assert_true(0));
	error = set_var(&env_tested, data->key_env, data->value_env);
	equal = str_array_equal(env_expected, env_tested);
	if (error != ERR_NONE)
		return(printf(FAIL_MSG, (char *) *case_name, "error"), assert_true(0));
	if (!equal)
		return(printf(FAIL_MSG, (char *) *case_name, "not equal"), assert_true(0));
	return (printf(SUCCESS_MSG, (char *) *case_name), assert_true(1));
}

t_test_result	execute_tests(void)
{
	const struct CMUnitTest test_cases[] = {
		cmocka_unit_test_prestate_setup_teardown(load_env_basic_tests, test_setup, test_teardown, env_cases[0].name),
		cmocka_unit_test_prestate_setup_teardown(load_env_basic_tests, test_setup, test_teardown, env_cases[1].name),
		cmocka_unit_test_prestate_setup_teardown(load_env_basic_tests, test_setup, test_teardown, env_cases[2].name),
		cmocka_unit_test_prestate_setup_teardown(get_env_basic_tests, test_setup, test_teardown, env_cases[3].name),
		cmocka_unit_test_prestate_setup_teardown(get_env_basic_tests, test_setup, test_teardown, env_cases[4].name),
		cmocka_unit_test_prestate_setup_teardown(get_env_basic_tests, test_setup, test_teardown, env_cases[5].name),
		cmocka_unit_test_prestate_setup_teardown(get_var_basic_tests, test_setup, test_teardown, env_cases[6].name),
		cmocka_unit_test_prestate_setup_teardown(get_var_basic_tests, test_setup, test_teardown, env_cases[7].name),
		cmocka_unit_test_prestate_setup_teardown(get_var_basic_tests, test_setup, test_teardown, env_cases[8].name),
		cmocka_unit_test_prestate_setup_teardown(get_var_basic_tests, test_setup, test_teardown, env_cases[9].name),
		cmocka_unit_test_prestate_setup_teardown(get_var_basic_tests, test_setup, test_teardown, env_cases[10].name),
		cmocka_unit_test_prestate_setup_teardown(remove_var_basic_tests, test_setup, test_teardown, env_cases[11].name),
		cmocka_unit_test_prestate_setup_teardown(remove_var_basic_tests, test_setup, test_teardown, env_cases[12].name),
		cmocka_unit_test_prestate_setup_teardown(remove_var_basic_tests, test_setup, test_teardown, env_cases[13].name),
		cmocka_unit_test_prestate_setup_teardown(remove_var_basic_tests, test_setup, test_teardown, env_cases[14].name),
		cmocka_unit_test_prestate_setup_teardown(remove_var_basic_tests, test_setup, test_teardown, env_cases[15].name),
		cmocka_unit_test_prestate_setup_teardown(set_var_basic_tests, test_setup, test_teardown, env_cases[16].name),
		cmocka_unit_test_prestate_setup_teardown(set_var_basic_tests, test_setup, test_teardown, env_cases[17].name),
		cmocka_unit_test_prestate_setup_teardown(set_var_basic_tests, test_setup, test_teardown, env_cases[18].name),
		cmocka_unit_test_prestate_setup_teardown(set_var_basic_tests, test_setup, test_teardown, env_cases[19].name),
		cmocka_unit_test_prestate_setup_teardown(set_var_basic_tests, test_setup, test_teardown, env_cases[20].name),
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
