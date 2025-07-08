/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 10:58:47 by apierret          #+#    #+#             */
/*   Updated: 2025/07/08 11:46:18 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <linux/limits.h>

#include "builtins.h"
#include "env.h"
#include "utils.h"

static t_error	cd_home(int *ret, t_hash_table *env)
{
	char	*path;
	t_error	err;

	if (ret == NULL || env == NULL)
		return (error (ERR_IMPLEMENTATION, NULL));
	err = get_var(&path, env, "HOME");
	if (err.id != ERR_NONE)
		return (*ret = 1, err);
	if (str_empty(path))
		return (*ret = 0, error(ERR_NONE, NULL));
	err = check_file("azbnaz", 0, 0, 0);
	if (err.id != ERR_NONE)
		return (*ret = 1, err);
	return (*ret = 0, error(ERR_NONE, NULL));
}

t_error		builtin_cd(int *ret, t_btin_data data, t_hash_table *env)
{
	int		value;
	t_error	err;

	if (ret == NULL || data.argv == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	value = 0;
	err = error(ERR_NONE, NULL);
	if (data.argc == 1)
		err = cd_home(&value, env);
	if (err.id != ERR_NONE)
		return (*ret = value, err);
	return (*ret = value, error(ERR_NONE, NULL));
}