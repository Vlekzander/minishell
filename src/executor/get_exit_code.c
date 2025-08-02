/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_exit_code.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 14:26:12 by apierret          #+#    #+#             */
/*   Updated: 2025/08/02 17:21:02 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include "executor.h"
#include "signals.h"

extern int	g_signal;

static int	exit_code_err(t_ret ret, t_error err)
{
	int	exit_code;

	exit_code = 0;
	if (ret.type == RET_VALUE)
		exit_code = ret.value;
	if (err.id == ERR_NO_EXEC)
		exit_code = 0;
	else if (err.id == ERR_PERMISSION || err.id == ERR_IS_DIRECTORY
		|| ret.ret_sub == RET_SUB_EXEC)
		exit_code = 126;
	else if (err.id == ERR_FILE_NOT_FOUND || err.id == ERR_CMD_NOT_FOUND)
		exit_code = 127;
	else if (err.id != ERR_NONE && err.id != ERR_EXIT)
		return (1);
	return (exit_code);
}

static int	exit_code(t_ret ret, t_error err, int status)
{
	int	exit_code;
	int	sig;

	exit_code = 0;
	setup_signals(0);
	if (ret.type == RET_PID && ret.pid != -1)
	{
		if (WIFEXITED(status))
			exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			if (sig == SIGQUIT)
				write(2, "Quit (core dumped)\n", 19);
			exit_code = 128 + sig;
		}
		return (g_signal = 0, exit_code);
	}
	exit_code = exit_code_err(ret, err);
	return (g_signal = 0, exit_code);
}

int	get_exit_code(t_ret ret, t_error err)
{
	int	status;

	status = 0;
	if (ret.type == RET_PID && ret.pid != -1)
	{
		waitpid(ret.pid, &status, 0);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		return (exit_code(ret, err, status));
	}
	return (exit_code(ret, err, status));
}
