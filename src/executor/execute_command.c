/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 14:03:02 by apierret          #+#    #+#             */
/*   Updated: 2025/08/05 20:30:08 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
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
	if (in_pipeline(redirs))
	{
		cmd->forked = 1;
		signal(SIGPIPE, sig_handler);
	}
	err = get_std_backup(&cmd->stdin, &cmd->stdout);
	if (err.id != ERR_NONE)
		return (err);
	err = handle_redirs(redirs, cmd->stdin, cmd->stdout);
	if (err.id == ERR_NONE)
		err = cmd->builtin(&ret->value, cmd, env);
	close_fd(cmd->stdin);
	close_fd(cmd->stdout);
	if (err.id == ERR_WRITE && g_signal == SIGPIPE)
		err = error(ERR_NONE, NULL);
	return (err);
}

static t_error	exec_cmd(t_ret *ret, t_list *args, t_list *redirs,
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
		if (in_pipeline(redirs))
			err.exit = 1;
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

static t_error	exec_fork(t_ret *ret, t_list *args, t_list *redirs,
					t_hash_table *env)
{
	pid_t		pid;

	if (ret == NULL || args == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	ret->type = RET_PID;
	pid = fork();
	if (pid == -1)
		return (ret->pid = -1, error(ERR_FORK, NULL));
	if (pid == 0)
		return (restore_signals(), exec_cmd(ret, args, redirs, env));
	ret->pid = pid;
	return (error(ERR_NONE, NULL));
}

t_error	execute_command(t_ret *ret, t_list **args, t_list *redirs,
			t_hash_table *env)
{
	t_error	err;

	if (ret == NULL || args == NULL || *args == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = prepare_redirs(redirs, env);
	if (err.id != ERR_NONE)
		return (ret->redir_error = 1, close_redirs(redirs), err);
	err = expand_list(args, env);
	if (err.id != ERR_NONE)
		return (close_redirs(redirs), err);
	if (*args == NULL)
		return (error(ERR_EMPTY, NULL));
	if (get_builtin((*args)->content) != NULL && !in_pipeline(redirs))
	{
		err = exec_cmd(ret, *args, redirs, env);
		return (close_redirs(redirs), err);
	}
	err = exec_fork(ret, *args, redirs, env);
	if (err.id != ERR_NONE)
		return (close_redirs(redirs), err);
	return (close_redirs(redirs), err);
}
