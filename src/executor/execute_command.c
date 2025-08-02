/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 14:03:02 by apierret          #+#    #+#             */
/*   Updated: 2025/08/02 19:02:18 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "executor.h"
#include "expansion.h"
#include "redirs.h"
#include "signals.h"
#include "utils.h"

static t_error	get_std_backup(int *stdin, int *stdout)
{
	if (stdin == NULL || stdout == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	*stdin = dup(STDIN_FILENO);
	if (*stdin == -1)
		return (error(ERR_ERRNO, NULL));
	*stdout = dup(STDOUT_FILENO);
	if (*stdout == -1)
		return (close_fd(*stdin), error(ERR_ERRNO, NULL));
	return (error(ERR_NONE, NULL));
}

static t_error	exec_builtin(t_ret *ret, t_command *cmd, t_list *redirs,
			t_hash_table *env)
{
	t_error		err;

	if (ret == NULL || cmd == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = get_std_backup(&cmd->stdin, &cmd->stdout);
	if (err.id != ERR_NONE)
		return (err);
	err = handle_redirs(redirs, cmd->stdin, cmd->stdout);
	if (err.id == ERR_NONE)
		err = cmd->builtin(&ret->value, cmd, env);
	close_fd(cmd->stdin);
	close_fd(cmd->stdout);
	return (err);
}

t_error	execute_command(t_ret *ret, t_list *args, t_list *redirs,
			t_hash_table *env)
{
	t_command	*cmd;
	t_error		err;

	if (ret == NULL || args == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	ret->type = RET_VALUE;
	err = prepare_command(&cmd, args, env);
	if (err.id != ERR_NONE)
		return (ret->ret_sub = RET_SUB_PREPARE, err);
	if (cmd->type == CMD_BUILTIN)
	{
		err = exec_builtin(ret, cmd, redirs, env);
		return (free_command(cmd), err);
	}
	err = handle_redirs(redirs, STDIN_FILENO, STDOUT_FILENO);
	restore_signals();
	if (err.id == ERR_NONE)
	{
		if (execve(cmd->executable, cmd->argv, cmd->envp) == -1)
			err = error(ERR_ERRNO, NULL);
		return (free_command(cmd), ret->ret_sub = RET_SUB_EXEC, err);
	}
	return (free_command(cmd), ret->ret_sub = RET_SUB_PREPARE, err);
}
