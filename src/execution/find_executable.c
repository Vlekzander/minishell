/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_executable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 00:47:59 by apierret          #+#    #+#             */
/*   Updated: 2025/07/03 12:11:40 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "env.h"
#include "execution.h"
#include "utils.h"

static t_error	check_path(char **executable, char *command)
{
	char		*str;
	struct stat	path_stat;

	if (executable == NULL || command == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (access(command, F_OK) == -1)
		return (error(ERR_FILE_NOT_FOUND, command));
	if (stat(command, &path_stat) != 0)
		return (error(ERR_ERRNO, command));
	if (S_ISDIR(path_stat.st_mode))
		return (error(ERR_IS_DIRECTORY, command));
	if (access(command, X_OK) == -1)
		return (error(ERR_PERMISSION, command));
	str = ft_strdup(command);
	if (str == NULL)
		return (error(ERR_ALLOCATION, NULL));
	return (*executable = str, error(ERR_NONE, NULL));
}

static t_error	check_env_path(char **executable, char *command, char **path)
{
	char			*str;
	t_strbuilder	*sb;
	t_error			err;

	if (executable == NULL || command == NULL || path == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	sb = create_strbuilder(64);
	if (sb == NULL)
		return (error(ERR_ALLOCATION, NULL));
	while (*path != NULL)
	{
		if (!strbuilder_append(sb, *(path++)) || !strbuilder_append(sb, "/")
			|| !strbuilder_append(sb, command))
			return (free_strbuilder(sb), error(ERR_ALLOCATION, NULL));
		err = check_path(&str, sb->buffer);
		if (err.id == ERR_PERMISSION)
			return (free_strbuilder(sb), free(err.cause), error(err.id, NULL));
		if (err.id == ERR_NONE && str == NULL)
			return (free_strbuilder(sb), error(ERR_ALLOCATION, NULL));
		if (err.id == ERR_NONE)
			return (*executable = str, free_strbuilder(sb), err);
		free(err.cause);
		sb->length = 0;
	}
	return (free_strbuilder(sb), error(ERR_CMD_NOT_FOUND, command));
}

t_error	find_executable(char **executable, char *command, t_hash_table *env)
{
	char	*str;
	char	**path;
	t_error	err;

	if (executable == NULL || command == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (ft_strchr(command, '/') != NULL)
		err = check_path(&str, command);
	else
	{
		err = get_var(&str, env, "PATH");
		if (err.id != ERR_NONE)
			return (err);
		path = ft_split(str, ':');
		if (path == NULL)
			return (error(ERR_ALLOCATION, NULL));
		err = check_env_path(&str, command, path);
		free_ddarray((void **) path);
	}
	if (err.id != ERR_NONE)
		return (err);
	return (*executable = str, error(ERR_NONE, NULL));
}
