/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:31:04 by apierret          #+#    #+#             */
/*   Updated: 2025/07/08 19:08:12 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "builtins.h"
#include "data.h"
#include "env.h"
#include "utils.h"

static t_error	export_var(char *str, t_hash_table *env)
{
	char	*ptr;
	char	*var;
	t_error	err;

	if (str == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (str[0] == '?' || str[0] == '=')
		return (error(ERR_INVALID_KEY, str));
	ptr = ft_strchr(str, '=');
	if (ptr == NULL)
		return (set_var(env, str, NULL));
	*ptr = '\0';
	if (ptr[-1] == '+')
	{
		ptr[-1] = '\0';
		err = get_var(&var, env, str);
		if (err.id != ERR_NONE)
			return (ptr[-1] = '+', *ptr = '=', err);
		var = ft_strjoin(var, ptr + 1);
		if (var == NULL)
			return (ptr[-1] = '+', *ptr = '=', error(ERR_ALLOCATION, NULL));
		return (err = set_var(env, str, var), free(var), ptr[-1] = '+', err);
	}
	err = set_var(env, str, ptr + 1);
	return (*ptr = '=', err);
}

static t_error	export_vars(int *ret, int argc, char **argv, t_hash_table *env)
{
	int		i;
	int		value;
	t_error	err;

	if (ret == NULL || argv == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	value = 0;
	err = error(ERR_NONE, NULL);
	i = 1;
	while (i < argc)
	{
		err = export_var(argv[i], env);
		if (err.id != ERR_NONE)
		{
			value = 1;
			print_error(err, "export");
		}
		i++;
	}
	return (*ret = value, err);
}

t_error	builtin_export(int *ret, t_btin_data data, t_hash_table *env)
{
	char	*str;
	int		value;
	t_error	err;

	if (ret == NULL || data.argv == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (data.argc == 1)
	{
		err = get_env(&str, 1, env);
		if (err.id != ERR_NONE)
		{
			*ret = 1;
			return (print_error(err, "export"), error(ERR_NONE, NULL));
		}
		ft_putstr_fd(str, data.stdout);
		return (free(str), *ret = 0, error(ERR_NONE, NULL));
	}
	export_vars(&value, data.argc, data.argv, env);
	return (*ret = value, error(ERR_NONE, NULL));
}
