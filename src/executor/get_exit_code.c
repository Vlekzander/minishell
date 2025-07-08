/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_exit_code.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 21:33:34 by apierret          #+#    #+#             */
/*   Updated: 2025/07/08 22:44:34 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include "executor.h"

extern int	g_signal;

static int	exit_code(t_ret ret, t_error err, int status)
{
	int	exit_code;

	exit_code = 1;
	if (ret.type == RET_PID && ret.pid != -1)
	{
		if (WIFEXITED(status))
			exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			g_signal = 0;
			exit_code = 128 + WTERMSIG(status);
		}
		return (exit_code);
	}
	if (ret.type == RET_VALUE)
		exit_code = ret.value;
	if (err.id == ERR_CMD_EMPTY)
		exit_code = 0;
	if (err.id == ERR_PERMISSION || err.id == ERR_IS_DIRECTORY)
		exit_code = 126;
	else if (ret.redir_error && err.id == ERR_FILE_NOT_FOUND)
		exit_code = 1;
	else if (err.id == ERR_FILE_NOT_FOUND || err.id == ERR_CMD_NOT_FOUND)
		exit_code = 127;
	return (exit_code);
}

int	get_exit_code_pipe(t_ret *rets, int size, t_error err)
{
	int		i;
	int		status;

	status = 0;
	i = 0;
	while (i < size -1 && rets[i].type == RET_PID)
		waitpid(rets[i++].pid, &status, 0);
	waitpid(rets[i].pid, &status, 0);
	return (exit_code(rets[i], err, status));
}

int	get_exit_code(t_ret ret, t_error err)
{
	int	status;

	status = 0;
	if (ret.type == RET_PID && ret.pid != -1)
	{
		waitpid(ret.pid, &status, 0);
		return (exit_code(ret, err, status));
	}
	return (exit_code(ret, err, status));
}
