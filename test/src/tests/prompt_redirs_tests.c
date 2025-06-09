/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_tests.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by apierret          #+#    #+#             */
/*   Updated: 2025/06/10 00:13:48 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "redirs.h"
#include "test.h"
#include "test_utils.h"

static t_test_case prompt_redirs_case[] =
{
	{
		"single_line",
		&(t_prompt_redirs_case)
		{
			"END",
			{"hello world", "END", NULL}
		}
	},
	{
		"multiple_lines",
		&(t_prompt_redirs_case)
		{
			"END",
			{"hello", "world", "END", NULL}
		}
	},
	{
		"single_line_no_end",
		&(t_prompt_redirs_case)
		{
			"END",
			{"hello world", NULL}
		}
	},
	{
		"multiple_lines_no_end",
		&(t_prompt_redirs_case)
		{
			"END",
			{"hello", "world", NULL}
		}
	},
	{
		"file_str",
		&(t_prompt_redirs_case)
		{
			"END",
			{"VERY_LARGE", "END", NULL}
		}
	},
	{
		"max_pipe",
		&(t_prompt_redirs_case)
		{
			"END",
			{"MAX_PIPE", "END", NULL}
		}
	},
	{ NULL }
};

t_redir	*redir;
t_list	*node;

static int	test_setup(void **state)
{
	(void) state;
	redir = create_redir(REDIR_HEREDOC, "END", 0);
	if (redir == NULL)
		return (-1);
	node = ft_lstnew(redir);
	if (node == NULL)
		return (free_redir(redir), -1);
	return (0);
}

static int	test_teardown(void **state) {
	(void) state;
	free_redir(redir);
	ft_lstdelone(node, NULL);
	return (0);
}

char	*get_str(char *line)
{
	char	*str;

	str = ft_strdup(line);
	if (str_equal(line, "VERY_LARGE"))
	{
		free(str);
		str = ft_calloc(123456 + 1, sizeof(char));
		ft_memset(str, 'X', 123456);
	}
	else if (str_equal(line, "MAX_PIPE"))
	{
		free(str);
		str = ft_calloc(65534 + 1, sizeof(char));
		ft_memset(str, 'X', 65534);
	}
	return (str);
}

char	*__wrap_readline (const char *prompt)
{
	(void) prompt;
	return (mock_type(char *));
}

static void prompt_redirs_tests(void **case_name)
{
	t_test_case				*tc;
	t_prompt_redirs_case	*data;
	t_error					error;
	char					*str;
	int						i;
	int						equal;

	if (case_name == NULL)
		return(printf("Implementation error.\n"), assert_true(0));
	tc = find_case(prompt_redirs_case, *case_name);
	if (tc == NULL)
		return (printf(CASE_NOT_FOUND_MSG, (char *) *case_name), assert_true(0));
	data = tc->data;
	str = ft_strdup(data->eof);
	if (str == NULL)
		return(printf(FAIL_MSG, (char *) *case_name, "allocation"), assert_true(0));
	free(redir->heredoc);
	redir->heredoc = str;
	for (i = 0; data->lines[i] != NULL; i++)
		will_return(__wrap_readline, get_str(data->lines[i]));
	if (!str_equal(redir->heredoc, data->lines[i-1]))
		will_return(__wrap_readline, NULL);
	error = prompt_redirs(node);
	if (error != ERR_NONE)
		return(printf(FAIL_MSG, (char *) *case_name, "error"), assert_true(0));
	i = 0;
	equal = 1;
	str = get_next_line(redir->fd);
	while (str != NULL)
	{
		char *line = get_str(data->lines[i]);
		if (ft_strncmp(line, str, ft_strlen(line)) != 0)
			equal = 0;
		free(line);
		free(str);
		i++;
		str = get_next_line(redir->fd);
	}
	if (equal == 0)
		return(printf(FAIL_MSG, (char *) *case_name, "equal"), assert_true(0));
	return (printf(SUCCESS_MSG, (char *) *case_name), assert_true(1));
}

t_test_result	execute_tests(void)
{
	const struct CMUnitTest test_cases[] = {
		cmocka_unit_test_prestate_setup_teardown(prompt_redirs_tests, test_setup, test_teardown, prompt_redirs_case[0].name),
		cmocka_unit_test_prestate_setup_teardown(prompt_redirs_tests, test_setup, test_teardown, prompt_redirs_case[1].name),
		cmocka_unit_test_prestate_setup_teardown(prompt_redirs_tests, test_setup, test_teardown, prompt_redirs_case[2].name),
		cmocka_unit_test_prestate_setup_teardown(prompt_redirs_tests, test_setup, test_teardown, prompt_redirs_case[3].name),
		cmocka_unit_test_prestate_setup_teardown(prompt_redirs_tests, test_setup, test_teardown, prompt_redirs_case[4].name),
		cmocka_unit_test_prestate_setup_teardown(prompt_redirs_tests, test_setup, test_teardown, prompt_redirs_case[5].name),
	};
	char	name[] = "redirs/prompt_redirs";
	t_test_result	result;

	printf(GROUP_HEADER, name);
	result.total = sizeof(test_cases)/sizeof(struct CMUnitTest);
	result.failed = cmocka_run_group_tests_name(name, test_cases, NULL, NULL);
	result.successful = result.total - result.failed;
	printf(GROUP_RESULT, name, result.successful, result.total);
	return (result);
}
