/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:16:01 by apierret          #+#    #+#             */
/*   Updated: 2025/06/22 20:12:20 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"
#include "env.h"

static int	env_to_str(char *key, void *value, void *data)
{
	t_strbuilder	*sb;
	int				ret;

	if (key == NULL || value == NULL || data == NULL)
		return (-1);
	sb = data;
	ret = strbuilder_append(sb, key);
	if (ret != 0)
		return (ret);
	ret = strbuilder_append(sb, "=");
	if (ret != 0)
		return (ret);
	ret = strbuilder_append(sb, value);
	if (ret != 0)
		return (ret);
	ret = strbuilder_append(sb, "\n");
	return (ret);
}

t_error	get_env(char **str, t_hash_table *env)
{
	t_strbuilder	*sb;
	char			*env_str;
	int				ret;

	if (str == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	sb = create_strbuilder(256);
	if (sb == NULL)
		return (error(ERR_ALLOCATION, NULL));
	ret = htable_foreach(env, env_to_str, sb);
	if (ret != 0)
		return (free_strbuilder(sb), error(ERR_ALLOCATION, NULL));
	env_str = ft_strdup(sb->buffer);
	if (env_str == NULL)
		return (free_strbuilder(sb), error(ERR_ALLOCATION, NULL));
	return (*str = env_str, free_strbuilder(sb), error(ERR_NONE, NULL));
}
