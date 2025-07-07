/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 18:29:57 by apierret          #+#    #+#             */
/*   Updated: 2025/07/07 12:23:15 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "utils.h"

void	close_fd(int fd)
{
	if (fd != -1)
		close(fd);
}

void	close_fds(int fd1, int fd2)
{
	close_fd(fd1);
	close_fd(fd2);
}

void	close_pipe(int *pipe)
{
	if (pipe == NULL)
		return ;
	close_fd(pipe[0]);
	close_fd(pipe[1]);
}

void	close_set(int *fd, int value)
{
	if (fd == NULL)
		return ;
	close_fd(*fd);
	*fd = value;
}
