/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 23:40:30 by apierret          #+#    #+#             */
/*   Updated: 2025/06/10 10:41:10 by apierret         ###   ########.fr       */
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
	t_error	error;

	error = ERR_NONE;
	slash = ft_strrchr(path, '/');
	if (slash != NULL && slash != path)
	{
		*slash = '\0';
		if (access(path, F_OK) == -1)
			error = ERR_FILE_NOT_FOUND;
		else if (access(path, W_OK) == -1)
			error = ERR_PERMISSION;
		*slash = '/';
	}
	return (error);
}

static t_error	check_file(char *path, t_redir_type type)
{
	struct stat	path_stat;

	if (path == NULL)
		return (ERR_IMPLEMENTATION);
	if (access(path, F_OK) == 0)
	{
		if (stat(path, &path_stat) != 0)
			return (ERR_ERRNO);
		if (S_ISDIR(path_stat.st_mode))
			return (ERR_IS_DIRECTORY);
	}
	if (type == REDIR_IN)
	{
		if (access(path, F_OK) == -1)
			return (ERR_FILE_NOT_FOUND);
		if (access(path, R_OK) == -1)
			return (ERR_PERMISSION);
		return (ERR_NONE);
	}
	if (access(path, F_OK) == 0 && access(path, W_OK) == -1)
		return (ERR_PERMISSION);
	return (check_subpath(path));
}

static t_error	prepare_of(t_redir_type type, int mode,
		int *flags, int *perms)
{
	if (flags == NULL || perms == NULL)
		return (ERR_IMPLEMENTATION);
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
	return (ERR_NONE);
}

t_error	open_file(int *fd, char *path, t_redir_type type, int mode)
{
	int		file;
	int		flags;
	int		perms;
	t_error	error;

	if (fd == NULL || path == NULL)
		return (ERR_IMPLEMENTATION);
	error = check_file(path, type);
	if (error != ERR_NONE)
		return (error);
	error = prepare_of(type, mode, &flags, &perms);
	if (error != ERR_NONE)
		return (error);
	file = open(path, flags, perms);
	if (file == -1)
		return (ERR_ERRNO);
	return (*fd = file, ERR_NONE);
}
