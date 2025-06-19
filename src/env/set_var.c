/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:16:14 by apierret          #+#    #+#             */
/*   Updated: 2025/06/19 11:50:21 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "env.h"

static int	is_valid_key(char *key)
{
	size_t	i;

	if (key == NULL)
		return (0);
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

	if (env == NULL || key == NULL || value == NULL)
		return (ERR_IMPLEMENTATION);
	if (!is_valid_key(key))
		return (ERR_INVALID_KEY);
	str_val = ft_strdup(value);
	if (str_val == NULL)
		return (ERR_ALLOCATION);
	if (htable_insert(env, key, str_val) != 0)
		return (free(str_val), ERR_ALLOCATION);
	return (ERR_NONE);
}
