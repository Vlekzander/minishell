/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_exit_code.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 21:33:34 by apierret          #+#    #+#             */
/*   Updated: 2025/07/07 12:31:13 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include "execution.h"

extern int	g_signal;

int	get_exit_code_pipe(t_ret *rets, int size, t_error err)
{
	int		i;

	i = 0;
	while (i < size - 1 && rets[i].type == RET_PID)
		waitpid(rets[i++].pid, NULL, 0);
	return (get_exit_code(rets[i], err));
}

int	get_exit_code(t_ret ret, t_error err)
{
	int	status;

	if (ret.type == RET_PID && ret.pid != -1)
	{
		status = 0;
		waitpid(ret.pid, &status, 0);
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			g_signal = 0;
			status = 128 + WTERMSIG(status);
		}
		return (status);
	}
	status = 1;
	if (ret.type == RET_VALUE)
		status = ret.value;
	if (err.id == ERR_PERMISSION)
		status = 126;
	else if (err.id == ERR_FILE_NOT_FOUND || err.id == ERR_CMD_NOT_FOUND)
		status = 127;
	return (status);
}
