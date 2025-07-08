/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:16:01 by apierret          #+#    #+#             */
/*   Updated: 2025/07/08 22:57:57 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"
#include "env.h"
#include "utils.h"

static t_error	sort_export(t_strbuilder *sb)
{
	char			**array;
	int				i;

	array = ft_split(sb->buffer, '\n');
	if (array == NULL)
		return (error(ERR_ALLOCATION, NULL));
	sort_array(array, export_char_order);
	sb->length = 0;
	i = 0;
	while (array[i] != NULL)
	{
		if (!strbuilder_append(sb, array[i]))
			return (free_ddarray((void **) array), error(ERR_ALLOCATION, NULL));
		if (!strbuilder_append(sb, "\n"))
			return (free_ddarray((void **) array), error(ERR_ALLOCATION, NULL));
		i++;
	}
	return (free_ddarray((void **) array), error(ERR_NONE, NULL));
}

static int	append_value(t_strbuilder *sb, char *value)
{
	int		i;
	char	str[2];

	if (sb == NULL || value == NULL)
		return (0);
	str[1] = '\0';
	i = 0;
	while (value[i] != '\0')
	{
		str[0] = value[i];
		if (value[i] == '\"')
		{
			if (!strbuilder_append(sb, "\\"))
				return (0);
		}
		if (!strbuilder_append(sb, str))
			return (0);
		i++;
	}
	return (1);
}

static int	export_format(char *key, void *value, void *data)
{
	t_strbuilder	*sb;

	if (key == NULL || data == NULL)
		return (0);
	sb = data;
	if (ft_strncmp("?", key, 2) == 0 || ft_strncmp("_", key, 2) == 0)
		return (1);
	if (!strbuilder_append(sb, "declare -x "))
		return (0);
	if (!strbuilder_append(sb, key))
		return (0);
	if (value != NULL)
	{
		if (!strbuilder_append(sb, "="))
			return (0);
		if (!strbuilder_append(sb, "\""))
			return (0);
		if (!append_value(sb, value))
			return (0);
		if (!strbuilder_append(sb, "\""))
			return (0);
	}
	if (!strbuilder_append(sb, "\n"))
		return (0);
	return (1);
}

static int	env_format(char *key, void *value, void *data)
{
	t_strbuilder	*sb;

	if (key == NULL || data == NULL)
		return (0);
	if (value == NULL)
		return (1);
	sb = data;
	if (ft_strncmp("?", key, 2) == 0)
		return (1);
	if (!strbuilder_append(sb, key))
		return (0);
	if (!strbuilder_append(sb, "="))
		return (0);
	if (!strbuilder_append(sb, value))
		return (0);
	if (!strbuilder_append(sb, "\n"))
		return (0);
	return (1);
}

t_error	get_env(char **str, int export, t_hash_table *env)
{
	t_strbuilder	*sb;
	char			*env_str;
	t_error			err;

	if (str == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	sb = create_strbuilder(256);
	if (sb == NULL)
		return (error(ERR_ALLOCATION, NULL));
	if (export)
	{
		if (!htable_foreach(env, export_format, sb))
			return (free_strbuilder(sb), error(ERR_ALLOCATION, NULL));
		err = sort_export(sb);
		if (err.id != ERR_NONE)
			return (free_strbuilder(sb), err);
	}
	else if (!htable_foreach(env, env_format, sb))
		return (free_strbuilder(sb), error(ERR_ALLOCATION, NULL));
	env_str = ft_strdup(sb->buffer);
	if (env_str == NULL)
		return (free_strbuilder(sb), error(ERR_ALLOCATION, NULL));
	return (*str = env_str, free_strbuilder(sb), error(ERR_NONE, NULL));
}
