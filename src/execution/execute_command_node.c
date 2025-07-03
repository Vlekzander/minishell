/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_node.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 19:34:24 by apierret          #+#    #+#             */
/*   Updated: 2025/07/03 23:54:20 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "env.h"
#include "execution.h"
#include "expand.h"
#include "redirs.h"

static int	get_exit_code(int status)
{
	int	ret;

	ret = 0;
	if (WIFEXITED(status))
		ret = WEXITSTATUS(status);
	return (ret);
}

static t_error	execute_command(int *ret, t_command *command, t_list *redirs,
	t_hash_table *env)
{
	pid_t	pid;
	int		status;
	t_error	err;

	if (command == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = prepare_redirs(redirs, env);
	if (err.id != ERR_NONE)
		return (*ret = 1, err);
	pid = fork();
	if (pid == -1)
		return (error(ERR_FORK, NULL));
	if (pid == 0)
	{
		err = handle_redirs(redirs, 1);
		if (err.id == ERR_NONE)
			execve(command->executable, command->args, command->envp);
		*ret = 1;
		exit(EXIT_FAILURE);
	}
	waitpid(pid, &status, 0);
	return (*ret = get_exit_code(status), error(ERR_NONE, NULL));
}

t_error	execute_command_node(t_ast *node, t_hash_table *env)
{
	t_command	*command;
	t_error		err;

	if (node == NULL || env == NULL || node->type != NODE_COMMAND)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = prepare_cmd(&command, node->command_args, env);
	if (err.id != ERR_NONE)
	{
		if (err.id == ERR_PERMISSION || err.id == ERR_IS_DIRECTORY)
			node->exit_code = 126;
		else if (err.id == ERR_CMD_NOT_FOUND)
			node->exit_code = 127;
		return (err);
	}
	err = execute_command(&node->exit_code, command, node->redirs, env);
	if (err.id != ERR_NONE)
		return (free_command(command), err);
	return (free_command(command), error(ERR_NONE, NULL));
}
