/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:26:15 by apierret          #+#    #+#             */
/*   Updated: 2025/07/03 17:26:54 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/stat.h>
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

t_error	check_file(char *path, t_redir_type type)
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
