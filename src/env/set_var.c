/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:16:14 by apierret          #+#    #+#             */
/*   Updated: 2025/07/08 18:41:55 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "env.h"

static int	is_valid_key(char *key)
{
	size_t	i;

	if (key == NULL)
		return (0);
	if (ft_strncmp("?", key, 2) == 0)
		return (1);
	i = 0;
	while (key[i] != '\0')
	{
		if (i == 0 && !ft_isalpha(key[i]) && key[i] != '_')
			return (0);
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

t_error	set_var(t_hash_table *env, char *key, char *value)
{
	char	*str_val;

	if (env == NULL || key == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (!is_valid_key(key))
		return (error(ERR_INVALID_KEY, key));
	str_val = ft_strdup(value);
	if (value != NULL && str_val == NULL)
		return (error(ERR_ALLOCATION, NULL));
	if (htable_insert(env, key, str_val) != 0)
		return (free(str_val), error(ERR_ALLOCATION, NULL));
	return (error(ERR_NONE, NULL));
}
