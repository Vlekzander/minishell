/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:31:04 by apierret          #+#    #+#             */
/*   Updated: 2025/07/10 00:08:01 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "builtins.h"
#include "data.h"
#include "env.h"
#include "utils.h"

static t_error	append_var(t_hash_table *env, char *key, char *value)
{
	char	*var;
	t_error	err;

	if (env == NULL || key == NULL || value == NULL)
		return (error(ERR_ALLOCATION, NULL));
	err = get_var(&var, env, key);
	if (err.id != ERR_NONE)
		return (err);
	var = ft_strjoin(var, value);
	if (var == NULL)
		return (error(ERR_ALLOCATION, NULL));
	return (err = set_var(env, key, var), free(var), err);
}

static t_error	append_set_var(t_hash_table *env, char *str, char *ptr)
{
	t_error	err;

	if (env == NULL || str == NULL || ptr == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	*ptr = '\0';
	if (ft_strncmp("_", str, 2) == 0)
		return (*ptr = '=', error(ERR_NONE, NULL));
	if (ptr[-1] == '+')
	{
		ptr[-1] = '\0';
		err = append_var(env, str, ptr + 1);
		if (err.id != ERR_NONE)
			return (ptr[-1] = '+', *ptr = '=', err);
		return (ptr[-1] = '+', *ptr = '=', err);
	}
	return (err = set_var(env, str, ptr + 1), *ptr = '=', err);
}

static t_error	export_var(char *str, t_hash_table *env)
{
	char	*ptr;
	char	*var;

	if (str == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (str[0] == '?' || str[0] == '=')
		return (error(ERR_INVALID_KEY, str));
	ptr = ft_strchr(str, '=');
	if (ptr == NULL)
	{
		var = htable_get(env, str);
		if (var == NULL)
			return (set_var(env, str, NULL));
		return (error(ERR_NONE, NULL));
	}
	return (append_set_var(env, str, ptr));
}

static t_error	export_vars(int *ret, int argc, char **argv, t_hash_table *env)
{
	int		i;
	int		value;
	t_error	err;

	if (ret == NULL || argv == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	value = 0;
	i = 1;
	while (i < argc)
	{
		err = export_var(argv[i], env);
		if (err.id != ERR_NONE)
		{
			value = 1;
			if (err.id == ERR_INVALID_KEY)
			{
				free(err.src);
				err.src = prefix_suffix_str(argv[i], "`", "'");
			}
			print_error(err, "export");
		}
		i++;
	}
	return (*ret = value, error(ERR_NONE, NULL));
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
	err = export_vars(&value, data.argc, data.argv, env);
	if (err.id != ERR_NONE)
		print_error(err, "export");
	return (*ret = value, error(ERR_NONE, NULL));
}
