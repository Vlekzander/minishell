/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 23:32:33 by apierret          #+#    #+#             */
/*   Updated: 2025/07/07 12:02:10 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "env.h"
#include "executor.h"
#include "expand.h"
#include "redirs.h"
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
	err = prepare_redirs(redirs, env);
	if (err.id != ERR_NONE)
		return (err);
	pid = fork();
	if (pid == -1)
		return (error(ERR_FORK, NULL));
	if (pid == 0)
	{
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
	data.argc = cmd->argc;
	data.argv = cmd->argv;
	err = prepare_redirs(redirs, env);
	if (err.id != ERR_NONE)
		return (err);
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
	data.argc = cmd->argc;
	data.argv = cmd->argv;
	data.stdin = STDIN_FILENO;
	data.stdout = STDOUT_FILENO;
	err = prepare_redirs(redirs, env);
	if (err.id != ERR_NONE)
		return (err);
	pid = fork();
	if (pid == -1)
		return (error(ERR_FORK, NULL));
	if (pid == 0)
	{
		ret->type = RET_VALUE;
		err = handle_redirs(redirs, STDIN_FILENO, STDOUT_FILENO);
		if (err.id == ERR_NONE)
			cmd->builtin(&ret->value, data, env);
		return (error(ERR_EXIT, NULL));
	}
	return (ret->pid = pid, error(ERR_NONE, NULL));
}

t_error	execute_command(t_ret *ret, t_list **cmd_args, t_list *cmd_redirs,
			t_hash_table *env)
{
	t_command	*cmd;
	t_error		err;

	if (ret == NULL || cmd_args == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = prepare_command(&cmd, cmd_args, env);
	if (err.id != ERR_NONE)
		return (ret->pid = -1, err);
	ret->type = RET_VALUE;
	ret->value = 1;
	if (cmd->type == CMD_BINARY
		|| (cmd->type == CMD_BUILTIN && cmd_redirs != NULL
			&& cmd_redirs->content != NULL
			&& ((t_redir *) cmd_redirs->content)->type == REDIR_PIPE))
	{
		ret->type = RET_PID;
		ret->pid = -1;
	}
	if (cmd->type == CMD_BINARY)
		err = exec_binary(ret, cmd, cmd_redirs, env);
	else if (cmd->type == CMD_BUILTIN && ret->type == RET_VALUE)
		err = exec_builtin(ret, cmd, cmd_redirs, env);
	else if (cmd->type == CMD_BUILTIN && ret->type == RET_PID)
		err = exec_builtin_fork(ret, cmd, cmd_redirs, env);
	return (free_command(cmd), err);
}
