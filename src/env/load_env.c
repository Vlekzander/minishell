/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:15:52 by apierret          #+#    #+#             */
/*   Updated: 2025/06/22 20:12:07 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "env.h"
#include "utils.h"

static t_error	load_var(t_hash_table *htable, char *env_str)
{
	char	*ptr;
	char	*key;
	t_error	err;

	if (htable == NULL || env_str == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	ptr = ft_strchr(env_str, '=');
	if (ptr != NULL)
	{
		key = ft_strndup(env_str, ptr - env_str);
		if (key == NULL)
			return (error(ERR_ALLOCATION, NULL));
		err = set_var(htable, key, ptr +1);
		if (err.code != ERR_NONE && err.code != ERR_INVALID_KEY)
			return (free(key), htable_destroy(htable), err);
		free(key);
	}
	return (error(ERR_NONE, NULL));
}

t_error	load_env(t_hash_table **env, char **envp)
{
	t_hash_table	*htable;
	size_t			i;
	t_error			err;

	if (env == NULL || envp == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	htable = htable_init(ENV_BUCKETS, NULL, free);
	if (htable == NULL)
		return (error(ERR_ALLOCATION, NULL));
	i = 0;
	while (envp[i] != NULL)
	{
		err = load_var(htable, envp[i]);
		if (err.code != ERR_NONE)
			return (err);
		i++;
	}
	return (*env = htable, error(ERR_NONE, NULL));
}
