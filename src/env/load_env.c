/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:15:52 by apierret          #+#    #+#             */
/*   Updated: 2025/06/19 12:12:13 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "env.h"
#include "utils.h"

static t_error	load_var(t_hash_table *htable, char *env_str)
{
	char	*ptr;
	char	*key;
	t_error	error;

	if (htable == NULL || env_str == NULL)
		return (ERR_IMPLEMENTATION);
	ptr = ft_strchr(env_str, '=');
	if (ptr != NULL)
	{
		key = ft_strndup(env_str, ptr - env_str);
		if (key == NULL)
			return (ERR_ALLOCATION);
		error = set_var(htable, key, ptr +1);
		if (error != ERR_NONE && error != ERR_INVALID_KEY)
			return (free(key), htable_destroy(htable), error);
		free(key);
	}
	return (ERR_NONE);
}

t_error	load_env(t_hash_table **env, char **envp)
{
	t_hash_table	*htable;
	size_t			i;
	t_error			error;

	if (env == NULL || envp == NULL)
		return (ERR_IMPLEMENTATION);
	htable = htable_init(ENV_BUCKETS, NULL, free);
	if (htable == NULL)
		return (ERR_ALLOCATION);
	i = 0;
	while (envp[i] != NULL)
	{
		error = load_var(htable, envp[i]);
		if (error != ERR_NONE)
			return (error);
		i++;
	}
	return (*env = htable, ERR_NONE);
}
