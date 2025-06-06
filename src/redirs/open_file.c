/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 23:40:30 by apierret          #+#    #+#             */
/*   Updated: 2025/06/06 12:28:06 by apierret         ###   ########.fr       */
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

static t_error	check_file(char *path, int write)
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
	if (!write)
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

t_error	open_file(int *fd, char *path, int write, int append)
{
	int		file;
	int		flags;
	t_error	error;

	if (fd == NULL || path == NULL)
		return (ERR_IMPLEMENTATION);
	error = check_file(path, write);
	if (error != ERR_NONE)
		return (error);
	flags = O_RDONLY;
	if (write)
	{
		flags = O_WRONLY | O_CREAT;
		if (append)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
	}
	file = open(path, flags, 0644);
	if (file == -1)
		return (ERR_ERRNO);
	return (*fd = file, ERR_NONE);
}
