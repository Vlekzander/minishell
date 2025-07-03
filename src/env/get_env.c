/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:16:01 by apierret          #+#    #+#             */
/*   Updated: 2025/07/03 14:19:05 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"
#include "env.h"

static int	env_to_str(char *key, void *value, void *data)
{
	t_strbuilder	*sb;

	if (key == NULL || value == NULL || data == NULL)
		return (0);
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

t_error	get_env(char **str, t_hash_table *env)
{
	t_strbuilder	*sb;
	char			*env_str;

	if (str == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	sb = create_strbuilder(256);
	if (sb == NULL)
		return (error(ERR_ALLOCATION, NULL));
	if (!htable_foreach(env, env_to_str, sb))
		return (free_strbuilder(sb), error(ERR_ALLOCATION, NULL));
	env_str = ft_strdup(sb->buffer);
	if (env_str == NULL)
		return (free_strbuilder(sb), error(ERR_ALLOCATION, NULL));
	return (*str = env_str, free_strbuilder(sb), error(ERR_NONE, NULL));
}
