/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 23:32:33 by apierret          #+#    #+#             */
/*   Updated: 2025/07/17 16:16:12 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "env.h"
#include "executor.h"
#include "expand.h"
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

static t_error	exec_binary(t_ret *ret, t_command *cmd, t_list *redirs,
			t_hash_table *env)
{
	pid_t	pid;
	t_error	err;

	if (ret == NULL || cmd == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	pid = fork();
	if (pid == -1)
		return (error(ERR_FORK, NULL));
	if (pid == 0)
	{
		restore_signals();
		err = handle_redirs(redirs, STDIN_FILENO, STDOUT_FILENO);
		if (err.id == ERR_NONE)
			execve(cmd->executable, cmd->argv, cmd->envp);
		exit(EXIT_FAILURE);
	}
	return (ret->pid = pid, error(ERR_NONE, NULL));
}

static t_error	exec_builtin(t_ret *ret, t_command *cmd, t_list *redirs,
			t_hash_table *env)
{
	t_btin_data	data;
	t_error		err;

	if (ret == NULL || cmd == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	data.forked = 0;
	data.argc = cmd->argc;
	data.argv = cmd->argv;
	err = get_std_backup(&data.stdin, &data.stdout);
	if (err.id != ERR_NONE)
		return (err);
	err = handle_redirs(redirs, data.stdin, data.stdout);
	if (err.id == ERR_NONE)
		err = cmd->builtin(&ret->value, data, env);
	close_fd(data.stdin);
	close_fd(data.stdout);
	return (err);
}

static t_error	exec_builtin_fork(t_ret *ret, t_command *cmd, t_list *redirs,
			t_hash_table *env)
{
	pid_t		pid;
	t_btin_data	data;
	t_error		err;

	if (ret == NULL || cmd == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	data.forked = 1;
	data.stdin = STDIN_FILENO;
	data.stdout = STDOUT_FILENO;
	data.argc = cmd->argc;
	data.argv = cmd->argv;
	pid = fork();
	if (pid == -1)
		return (error(ERR_FORK, NULL));
	if (pid != 0)
		return (ret->pid = pid, error(ERR_NONE, NULL));
	ret->type = RET_VALUE;
	err = handle_redirs(redirs, STDIN_FILENO, STDOUT_FILENO);
	if (err.id == ERR_NONE)
		cmd->builtin(&ret->value, data, env);
	return (error(ERR_EXIT, NULL));
}

t_error	execute_command(t_ret *ret, t_list **cmd_args, t_list *redirs,
			t_hash_table *env)
{
	t_command	*cmd;
	t_error		err;

	if (ret == NULL || cmd_args == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = prepare_redirs(redirs, env);
	if (err.id != ERR_NONE)
		return (close_redirs(redirs), ret->redir_error = 1, err);
	err = prepare_command(&cmd, cmd_args, env);
	if (err.id != ERR_NONE)
		return (close_redirs(redirs), ret->type = RET_PID, ret->pid = -1, err);
	err = prepare_return(ret, cmd->type, redirs);
	if (err.id != ERR_NONE)
		return (close_redirs(redirs), ret->type = RET_PID, ret->pid = -1, err);
	if (cmd->type == CMD_BINARY)
		err = exec_binary(ret, cmd, redirs, env);
	else if (cmd->type == CMD_BUILTIN && ret->type == RET_PID)
		err = exec_builtin_fork(ret, cmd, redirs, env);
	else if (cmd->type == CMD_BUILTIN && ret->type == RET_VALUE)
		err = exec_builtin(ret, cmd, redirs, env);
	return (close_redirs(redirs), free_command(cmd), err);
}
