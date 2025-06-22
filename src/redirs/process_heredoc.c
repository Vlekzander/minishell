/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by apierret          #+#    #+#             */
/*   Updated: 2025/06/22 20:38:45 by apierret         ###   ########.fr       */
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
		return (error(ERR_IMPLEMENTATION, NULL));
	len = ft_strlen(FILE_PATH_PREFIX);
	name = ft_calloc(len + 5 + 1, sizeof(char));
	if (name == NULL)
		return (error(ERR_ALLOCATION, NULL));
	ptr = (unsigned long) &name;
	ft_memcpy(name, FILE_PATH_PREFIX, len);
	end = name + len;
	i = 0;
	while (i < 5)
	{
		end[i] = 'A' + ((ptr >> (i * 4)) % 26);
		i++;
	}
	return (*filename = name, error(ERR_NONE, NULL));
}

static t_error	buf_to_file(int *fd, t_strbuilder *sb)
{
	t_error	err;
	char	*path;

	if (fd == NULL || sb == NULL || sb->buffer == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = get_file_path(&path);
	if (err.code != ERR_NONE)
		return (err);
	err = open_file(fd, path, REDIR_HEREDOC, 1);
	if (err.code != ERR_NONE)
		return (err);
	write(*fd, sb->buffer, sb->length);
	close(*fd);
	err = open_file(fd, path, REDIR_HEREDOC, 0);
	if (err.code != ERR_NONE)
		return (err);
	unlink(path);
	free(path);
	return (error(ERR_NONE, NULL));
}

static t_error	buf_to_pipe(int *fd, t_strbuilder *sb)
{
	int	pipe_fds[2];

	if (fd == NULL || sb == NULL || sb->buffer == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (pipe(pipe_fds) == -1)
		return (error(ERR_PIPE, NULL));
	write(pipe_fds[1], sb->buffer, sb->length);
	close(pipe_fds[1]);
	return (*fd = pipe_fds[0], error(ERR_NONE, NULL));
}

t_error	process_heredoc(t_redir *redir, t_strbuilder *sb)
{
	int		fd;
	t_error	err;

	if (redir == NULL || sb == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (sb->length > 65535)
		err = buf_to_file(&fd, sb);
	else
		err = buf_to_pipe(&fd, sb);
	if (err.code != ERR_NONE)
		return (err);
	return (redir->fd = fd, error(ERR_NONE, NULL));
}
