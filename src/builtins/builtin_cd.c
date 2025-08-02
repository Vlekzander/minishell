/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 10:58:47 by apierret          #+#    #+#             */
/*   Updated: 2025/08/02 19:04:47 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <linux/limits.h>

#include "builtins.h"
#include "data.h"
#include "env.h"
#include "utils.h"

static t_error	change_dir(char *path)
{
	t_error	err;

	if (path == NULL)
		return (error (ERR_IMPLEMENTATION, NULL));
	if (ft_strlen(path) == 0)
		return (error(ERR_NONE, NULL));
	err = check_file(path, 0, 0, 0);
	if (err.id != ERR_NONE)
		return (err);
	if (chdir(path) == -1)
		return (error(ERR_ERRNO, path));
	return (error(ERR_NONE, NULL));
}

static t_error	cd_home(t_hash_table *env)
{
	char	*path;

	if (env == NULL)
		return (error (ERR_IMPLEMENTATION, NULL));
	path = (char *) htable_get(env, "HOME");
	if (path == NULL)
		return (error(ERR_HOME_NOT_SET, NULL));
	return (change_dir(path));
}

static t_error	process_cd(int argc, char **argv, t_hash_table *env)
{
	char	pwd[PATH_MAX];
	t_error	err;

	if (argv == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (getcwd(pwd, PATH_MAX) == NULL)
		return (error(ERR_ERRNO, pwd));
	if (argc == 1)
		err = cd_home(env);
	else if (argc == 2)
		err = change_dir(argv[1]);
	else
		err = (error(ERR_TOO_MANY_ARGS, NULL));
	if (err.id != ERR_NONE)
		return (err);
	err = set_var(env, "OLDPWD", pwd);
	if (err.id != ERR_NONE)
		return (err);
	if (getcwd(pwd, PATH_MAX) == NULL)
		return (error(ERR_ERRNO, pwd));
	err = set_var(env, "PWD", pwd);
	return (err);
}

t_error	builtin_cd(int *ret, void *data, t_hash_table *env)
{
	t_command	*cmd;
	int			value;
	t_error		err;

	cmd = (t_command *) data;
	if (ret == NULL || cmd->argv == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	value = 0;
	err = process_cd(cmd->argc, cmd->argv, env);
	if (err.id != ERR_NONE)
	{
		if (err.id == ERR_IMPLEMENTATION)
			return (err);
		print_error(err, "cd");
		value = 1;
	}
	return (*ret = value, error(ERR_NONE, NULL));
}
