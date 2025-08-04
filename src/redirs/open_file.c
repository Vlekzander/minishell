/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 23:40:30 by apierret          #+#    #+#             */
/*   Updated: 2025/08/04 19:23:57 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "libft.h"
#include "redirs.h"
#include "utils.h"

static t_error	prepare_of(t_redir_type type, int mode,
							int *flags, int *perms)
{
	if (flags == NULL || perms == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	*flags = O_RDONLY;
	*perms = 0644;
	if (type == REDIR_OUT)
	{
		*flags = O_WRONLY | O_CREAT;
		if (mode)
			*flags |= O_APPEND;
		else
			*flags |= O_TRUNC;
	}
	else if (type == REDIR_HEREDOC)
	{
		if (mode)
			*flags = O_CREAT | O_WRONLY | O_EXCL | O_TRUNC;
		else
			*flags = O_RDONLY;
		*perms = 0600;
	}
	return (error(ERR_NONE, NULL));
}

static t_error	check_open_file(char *path, t_redir_type type, int mode)
{
	int	read;
	int	write;

	if (path == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	read = 0;
	write = 0;
	if (type == REDIR_IN || (type == REDIR_HEREDOC && !mode))
		read = 1;
	if (type == REDIR_OUT || (type == REDIR_HEREDOC && mode))
		write = 1;
	return (check_file(path, 1, read, write));
}

t_error	open_file(int *fd, char *path, t_redir_type type, int mode)
{
	int		file;
	int		flags;
	int		perms;
	t_error	err;

	if (fd == NULL || path == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = check_open_file(path, type, mode);
	if (err.id != ERR_NONE)
		return (err);
	err = prepare_of(type, mode, &flags, &perms);
	if (err.id != ERR_NONE)
		return (err);
	file = open(path, flags, perms);
	if (file == -1)
		return (error(ERR_ERRNO, path));
	return (*fd = file, error(ERR_NONE, NULL));
}
