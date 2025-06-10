/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by apierret          #+#    #+#             */
/*   Updated: 2025/06/10 11:58:24 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "redirs.h"
#define FILE_PATH_PREFIX "/tmp/msh-thd-"

static t_error	get_file_path(char **filename)
{
	char			*name;
	char			*end;
	size_t			i;
	size_t			len;
	unsigned long	ptr;

	if (filename == NULL)
		return (ERR_IMPLEMENTATION);
	len = ft_strlen(FILE_PATH_PREFIX);
	name = ft_calloc(len + 5 + 1, sizeof(char));
	if (name == NULL)
		return (ERR_ALLOCATION);
	ptr = (unsigned long) &name;
	ft_memcpy(name, FILE_PATH_PREFIX, len);
	end = name + len;
	i = 0;
	while (i < 5)
	{
		end[i] = 'A' + ((ptr >> (i * 4)) % 26);
		i++;
	}
	return (*filename = name, ERR_NONE);
}

static t_error	buf_to_file(int *fd, t_strbuilder *sb)
{
	t_error	error;
	char	*path;

	if (fd == NULL || sb == NULL || sb->buffer == NULL)
		return (ERR_IMPLEMENTATION);
	error = get_file_path(&path);
	if (error != ERR_NONE)
		return (error);
	error = open_file(fd, path, REDIR_HEREDOC, 1);
	if (error != ERR_NONE)
		return (error);
	write(*fd, sb->buffer, sb->length);
	close(*fd);
	error = open_file(fd, path, REDIR_HEREDOC, 0);
	if (error != ERR_NONE)
		return (error);
	unlink(path);
	free(path);
	return (ERR_NONE);
}

static t_error	buf_to_pipe(int *fd, t_strbuilder *sb)
{
	int	pipe_fds[2];

	if (fd == NULL || sb == NULL || sb->buffer == NULL)
		return (ERR_IMPLEMENTATION);
	if (pipe(pipe_fds) == -1)
		return (ERR_PIPE);
	write(pipe_fds[1], sb->buffer, sb->length);
	close(pipe_fds[1]);
	return (*fd = pipe_fds[0], ERR_NONE);
}

t_error	process_heredoc(t_redir *redir, t_strbuilder *sb)
{
	int		fd;
	t_error	error;

	if (redir == NULL || sb == NULL)
		return (ERR_IMPLEMENTATION);
	if (sb->length > 65535)
		error = buf_to_file(&fd, sb);
	else
		error = buf_to_pipe(&fd, sb);
	if (error != ERR_NONE)
		return (error);
	return (redir->fd = fd, ERR_NONE);
}
