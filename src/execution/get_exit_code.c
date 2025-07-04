/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_exit_code.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 21:33:34 by apierret          #+#    #+#             */
/*   Updated: 2025/07/05 01:14:41 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include "execution.h"

extern int	g_signal;

int	get_exit_code(pid_t pid, t_error err)
{
	int	status;

	if (pid != -1)
	{
		waitpid(pid, &status, 0);
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
	if (err.id == ERR_NONE)
		status = 0;
	else if (err.id == ERR_PERMISSION)
		status = 126;
	else if (err.id == ERR_FILE_NOT_FOUND || err.id == ERR_CMD_NOT_FOUND)
		status = 127;
	return (status);
}
