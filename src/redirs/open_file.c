/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 23:40:30 by apierret          #+#    #+#             */
/*   Updated: 2025/06/22 20:37:33 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "libft.h"
#include "redirs.h"

static t_error	check_subpath(char *path)
{
	char	*slash;
	t_error	err;

	err = error(ERR_NONE, NULL);
	slash = ft_strrchr(path, '/');
	if (slash != NULL && slash != path)
	{
		*slash = '\0';
		if (access(path, F_OK) == -1)
			err = error(ERR_FILE_NOT_FOUND, path);
		else if (access(path, W_OK) == -1)
			err = error(ERR_PERMISSION, path);
		*slash = '/';
	}
	return (err);
}

static t_error	check_file(char *path, t_redir_type type)
{
	struct stat	path_stat;

	if (path == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (access(path, F_OK) == 0)
	{
		if (stat(path, &path_stat) != 0)
			return (error(ERR_ERRNO, path));
		if (S_ISDIR(path_stat.st_mode))
			return (error(ERR_IS_DIRECTORY, path));
	}
	if (type == REDIR_IN)
	{
		if (access(path, F_OK) == -1)
			return (error(ERR_FILE_NOT_FOUND, path));
		if (access(path, R_OK) == -1)
			return (error(ERR_PERMISSION, path));
		return (error(ERR_NONE, NULL));
	}
	if (access(path, F_OK) == 0 && access(path, W_OK) == -1)
		return (error(ERR_PERMISSION, path));
	return (check_subpath(path));
}

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

t_error	open_file(int *fd, char *path, t_redir_type type, int mode)
{
	int		file;
	int		flags;
	int		perms;
	t_error	err;

	if (fd == NULL || path == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = check_file(path, type);
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
