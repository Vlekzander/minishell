/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_node.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 14:32:02 by apierret          #+#    #+#             */
/*   Updated: 2025/07/04 14:55:26 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include "execution.h"

static int	get_exit_code(int value)
{
	int	ret;

	ret = 0;
	if (WIFEXITED(value))
		ret = WEXITSTATUS(value);
	return (ret);
}

t_error	execute_command_node(t_ast *node, t_hash_table *env)
{
	pid_t	pid;
	int		val;
	t_error	err;

	if (node == NULL || env == NULL || node->type != NODE_COMMAND)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = execute_command(&pid, &node->command_args, node->redirs, env);
	if (err.id != ERR_NONE)
	{
		node->exit_code = 1;
		if (pid != -1)
		{
			if (err.id == ERR_PERMISSION || err.id == ERR_IS_DIRECTORY)
				node->exit_code = 126;
			else if (err.id == ERR_CMD_NOT_FOUND)
				node->exit_code = 127;
		}
		return (err);
	}
	if (pid != -1)
	{
		waitpid(pid, &val, 0);
		node->exit_code = get_exit_code(val);
	}
	else
		node->exit_code = 1;
	return (error(ERR_NONE, NULL));
}
