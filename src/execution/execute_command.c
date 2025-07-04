/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 23:32:33 by apierret          #+#    #+#             */
/*   Updated: 2025/07/04 14:59:17 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "env.h"
#include "execution.h"
#include "expand.h"
#include "redirs.h"

static t_error	run_exec(pid_t *pid_ret, t_command *command, t_list *redirs,
			t_hash_table *env)
{
	pid_t	pid;
	t_error	err;

	if (command == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = prepare_redirs(redirs, env);
	if (err.id != ERR_NONE)
		return (*pid_ret = -1, err);
	pid = fork();
	if (pid == -1)
		return (*pid_ret = -1, error(ERR_FORK, NULL));
	if (pid == 0)
	{
		err = handle_redirs(redirs, 1);
		if (err.id == ERR_NONE)
			execve(command->executable, command->args, command->envp);
		*pid_ret = -1;
		exit(EXIT_FAILURE);
	}
	return (*pid_ret = pid, error(ERR_NONE, NULL));
}

t_error	execute_command(pid_t *pid, t_list **cmd_args, t_list *cmd_redirs,
	t_hash_table *env)
{
	t_command	*command;
	t_error		err;

	if (pid == NULL || cmd_args == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = prepare_cmd(&command, cmd_args, env);
	if (err.id != ERR_NONE)
		return (*pid = -1, err);
	err = run_exec(pid, command, cmd_redirs, env);
	if (err.id != ERR_NONE)
		return (free_command(command), err);
	return (free_command(command), error(ERR_NONE, NULL));
}
