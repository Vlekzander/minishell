/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_exit_code.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 21:33:34 by apierret          #+#    #+#             */
/*   Updated: 2025/07/04 21:35:16 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include "execution.h"

int	get_exit_code(pid_t pid, t_error err)
{
	int	status;

	status = 0;
	if (pid != -1)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
		return (status);
	}
	if (err.id == ERR_NONE)
		return (0);
	if (err.id == ERR_PERMISSION)
		return (126);
	if (err.id == ERR_FILE_NOT_FOUND || err.id == ERR_CMD_NOT_FOUND)
		return (127);
	return (1);
}
