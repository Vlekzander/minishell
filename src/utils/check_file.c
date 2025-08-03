/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 11:09:48 by apierret          #+#    #+#             */
/*   Updated: 2025/08/03 14:36:13 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/stat.h>
#include "error.h"
#include "libft.h"

static t_error	check_subpath(char *path, int write)
{
	char	*slash;

	slash = ft_strrchr(path, '/');
	if (slash != NULL && slash != path)
	{
		*slash = '\0';
		if (access(path, F_OK) == -1)
			return (*slash = '/', error(ERR_FILE_NOT_FOUND, path));
		if (write && access(path, W_OK) == -1)
			return (*slash = '/', error(ERR_PERMISSION, path));
		*slash = '/';
	}
	else if (!write && access(path, F_OK) == -1)
		return (error(ERR_FILE_NOT_FOUND, path));
	return (error(ERR_NONE, NULL));
}

t_error	check_read_write(char *path, int read, int write)
{
	if (path == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (read)
	{
		if (access(path, F_OK) == -1)
			return (error(ERR_FILE_NOT_FOUND, path));
		if (access(path, R_OK) == -1)
			return (error(ERR_PERMISSION, path));
	}
	if (write)
	{
		if (access(path, F_OK) == 0 && access(path, W_OK) == -1)
			return (error(ERR_PERMISSION, path));
	}
	return (error(ERR_NONE, NULL));
}

t_error	check_file(char *path, int is_file, int read, int write)
{
	struct stat	path_stat;
	t_error		err;

	if (path == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (ft_strlen(path) == 0)
		return (error(ERR_FILE_NOT_FOUND, path));
	if (access(path, F_OK) == 0)
	{
		if (stat(path, &path_stat) != 0)
			return (error(ERR_ERRNO, path));
		if (S_ISDIR(path_stat.st_mode))
		{
			if (is_file)
				return (error(ERR_IS_DIRECTORY, path));
			if (access(path, X_OK) == -1)
				return (error(ERR_PERMISSION, path));
		}
	}
	err = check_read_write(path, read, write);
	if (err.id != ERR_NONE)
		return (err);
	if ((!is_file || write) && access(path, F_OK) == -1)
		err = check_subpath(path, write);
	return (err);
}
