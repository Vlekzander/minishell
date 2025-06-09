/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_file_tests.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 10:35:43 by apierret          #+#    #+#             */
/*   Updated: 2025/06/09 17:07:13 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "redirs.h"
#include "test.h"
#include "test_utils.h"

static t_test_case open_file_cases[] =
{
	{
		"read_file_all_perm",
		&(t_open_file_case)
		{
			.file = (t_file)
			{
				.name = "/tmp/file.txt",
				.perm = 0777,
				.is_folder = 0
			},
			.create = 1,
			.type = REDIR_IN,
			.append = 0,
			.expected_error = ERR_NONE
		}
	},
	{
		"write_file_all_perm",
		&(t_open_file_case)
		{
			.file = (t_file)
			{
				.name = "/tmp/file.txt",
				.perm = 0777,
				.is_folder = 0
			},
			.create = 1,
			.type = REDIR_OUT,
			.append = 0,
			.expected_error = ERR_NONE
		}
	},
	{
		"append_file_all_perm",
		&(t_open_file_case)
		{
			.file = (t_file)
			{
				.name = "/tmp/file.txt",
				.perm = 0777,
				.is_folder = 0
			},
			.create = 1,
			.type = REDIR_OUT,
			.append = 1,
			.expected_error = ERR_NONE
		}
	},
	{
		"read_file_not_found",
		&(t_open_file_case)
		{
			.file = (t_file)
			{
				.name = "/tmp/notfound.txt",
				.perm = 0777,
				.is_folder = 0
			},
			.create = 0,
			.type = REDIR_IN,
			.append = 0,
			.expected_error = ERR_FILE_NOT_FOUND
		}
	},
	{
		"no_read_perm",
		&(t_open_file_case)
		{
			.file = (t_file)
			{
				.name = "/tmp/no_read.txt",
				.perm = 0333,
				.is_folder = 0
			},
			.create = 1,
			.type = REDIR_IN,
			.append = 0,
			.expected_error = ERR_PERMISSION
		}
	},
	{
		"no_write_perm",
		&(t_open_file_case)
		{
			.file = (t_file)
			{
				.name = "/tmp/no_write.txt",
				.perm = 0555,
				.is_folder = 0
			},
			.create = 1,
			.type = REDIR_OUT,
			.append = 0,
			.expected_error = ERR_PERMISSION
		}
	},
	{
		"no_write_append_perm",
		&(t_open_file_case)
		{
			.file = (t_file)
			{
				.name = "/tmp/no_write_append.txt",
				.perm = 0555,
				.is_folder = 0
			},
			.create = 1,
			.type = REDIR_OUT,
			.append = 1,
			.expected_error = ERR_PERMISSION
		}
	},
	{
		"read_folder",
		&(t_open_file_case)
		{
			.file = (t_file)
			{
				.name = "/tmp/read_folder",
				.perm = 0777,
				.is_folder = 1
			},
			.create = 1,
			.type = REDIR_IN,
			.append = 0,
			.expected_error = ERR_IS_DIRECTORY
		}
	},
	{
		"write_folder",
		&(t_open_file_case)
		{
			.file = (t_file)
			{
				.name = "/tmp/write_folder",
				.perm = 0777,
				.is_folder = 1
			},
			.create = 1,
			.type = REDIR_OUT,
			.append = 0,
			.expected_error = ERR_IS_DIRECTORY
		}
	},
	{
		"append_folder",
		&(t_open_file_case)
		{
			.file = (t_file)
			{
				.name = "/tmp/append_folder",
				.perm = 0777,
				.is_folder = 1
			},
			.create = 1,
			.type = REDIR_OUT,
			.append = 1,
			.expected_error = ERR_IS_DIRECTORY
		}
	},
	{
		"read_path_not_found",
		&(t_open_file_case)
		{
			.file = (t_file)
			{
				.name = "./unknown/notfound.txt",
				.perm = 0777,
				.is_folder = 0
			},
			.create = 0,
			.type = REDIR_IN,
			.append = 0,
			.expected_error = ERR_FILE_NOT_FOUND
		}
	},
	{
		"write_path_not_found",
		&(t_open_file_case)
		{
			.file = (t_file)
			{
				.name = "./unknown/notfound.txt",
				.perm = 0777,
				.is_folder = 0
			},
			.create = 0,
			.type = REDIR_OUT,
			.append = 0,
			.expected_error = ERR_FILE_NOT_FOUND
		}
	},
	{
		"append_path_not_found",
		&(t_open_file_case)
		{
			.file = (t_file)
			{
				.name = "./unknown/notfound.txt",
				.perm = 0777,
				.is_folder = 0
			},
			.create = 0,
			.type = REDIR_OUT,
			.append = 1,
			.expected_error = ERR_FILE_NOT_FOUND
		}
	},
	{
		"write_subfolder_no_perm",
		&(t_open_file_case)
		{
			.file = (t_file)
			{
				.name = "/bin/write.txt",
				.perm = 0777,
				.is_folder = 0
			},
			.create = 0,
			.type = REDIR_OUT,
			.append = 0,
			.expected_error = ERR_PERMISSION
		}
	},
	{
		"append_subfolder_no_perm",
		&(t_open_file_case)
		{
			.file = (t_file)
			{
				.name = "/bin/append.txt",
				.perm = 0777,
				.is_folder = 0
			},
			.create = 0,
			.type = REDIR_OUT,
			.append = 1,
			.expected_error = ERR_PERMISSION
		}
	},
	{NULL}
};

static int		fd;
static t_error	error;
static t_file	*file;

static int	test_setup(void **state)
{
	(void) state;
	fd = -1;
	error = ERR_NONE;
	file = NULL;
	return (0);
}

static int	test_teardown(void **state) {
	(void) state;
	if (error == ERR_NONE)
		close(fd);
	delete_file(*file);
	return (0);
}

static void open_file_tests(void **case_name)
{
	t_test_case			*tc;
	t_open_file_case	*data;

	if (case_name == NULL)
		return(printf("Implementation error.\n"), assert_true(0));
	tc = find_case(open_file_cases, *case_name);
	if (tc == NULL)
		return (printf(CASE_NOT_FOUND_MSG, (char *) *case_name), assert_true(0));
	data = tc->data;
	file = &data->file;
	if (data->create)
	{
		if (create_file(*file) != ERR_NONE)
			return(printf(FAIL_MSG, (char *) *case_name, "create_file"), assert_true(0));
	}
	error =	open_file(&fd, file->name, data->type, data->append);
	if (error != data->expected_error)
		return(printf(FAIL_MSG, (char *) *case_name, "error"), assert_true(0));
	return (printf(SUCCESS_MSG, (char *) *case_name), assert_true(1));
}

t_test_result	execute_tests(void)
{
	const struct CMUnitTest test_cases[] = {
		cmocka_unit_test_prestate_setup_teardown(open_file_tests, test_setup, test_teardown, open_file_cases[0].name),
		cmocka_unit_test_prestate_setup_teardown(open_file_tests, test_setup, test_teardown, open_file_cases[1].name),
		cmocka_unit_test_prestate_setup_teardown(open_file_tests, test_setup, test_teardown, open_file_cases[2].name),
		cmocka_unit_test_prestate_setup_teardown(open_file_tests, test_setup, test_teardown, open_file_cases[3].name),
		cmocka_unit_test_prestate_setup_teardown(open_file_tests, test_setup, test_teardown, open_file_cases[4].name),
		cmocka_unit_test_prestate_setup_teardown(open_file_tests, test_setup, test_teardown, open_file_cases[5].name),
		cmocka_unit_test_prestate_setup_teardown(open_file_tests, test_setup, test_teardown, open_file_cases[6].name),
		cmocka_unit_test_prestate_setup_teardown(open_file_tests, test_setup, test_teardown, open_file_cases[7].name),
		cmocka_unit_test_prestate_setup_teardown(open_file_tests, test_setup, test_teardown, open_file_cases[8].name),
		cmocka_unit_test_prestate_setup_teardown(open_file_tests, test_setup, test_teardown, open_file_cases[9].name),
		cmocka_unit_test_prestate_setup_teardown(open_file_tests, test_setup, test_teardown, open_file_cases[10].name),
		cmocka_unit_test_prestate_setup_teardown(open_file_tests, test_setup, test_teardown, open_file_cases[11].name),
		cmocka_unit_test_prestate_setup_teardown(open_file_tests, test_setup, test_teardown, open_file_cases[12].name),
		cmocka_unit_test_prestate_setup_teardown(open_file_tests, test_setup, test_teardown, open_file_cases[13].name),
		cmocka_unit_test_prestate_setup_teardown(open_file_tests, test_setup, test_teardown, open_file_cases[14].name),
	};
	char	name[] = "redirs/open_file";
	t_test_result	result;

	printf(GROUP_HEADER, name);
	result.total = sizeof(test_cases)/sizeof(struct CMUnitTest);
	result.failed = cmocka_run_group_tests_name(name, test_cases, NULL, NULL);
	result.successful = result.total - result.failed;
	printf(GROUP_RESULT, name, result.successful, result.total);
	return (result);
}