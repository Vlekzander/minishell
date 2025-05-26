/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:16:34 by apierret          #+#    #+#             */
/*   Updated: 2025/05/26 13:21:01 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "env.h"
#include "utils.h"

static size_t	str_array_len(char **array)
{
	size_t	len;

	if (array == NULL)
		return (0);
	len = 0;
	while (array[len] != NULL)
		len++;
	return (len);
}

t_error	remove_var(char ***env, char *key)
{
	char	**array;
	char	*ptr;
	size_t	key_len;
	size_t	i;
	size_t	j;

	if (env == NULL || *env == NULL || key == NULL)
		return (ERR_IMPLEMENTATION);
	array = ft_calloc(str_array_len(*env) + 1, sizeof(char *));
	if (array == NULL)
		return (ERR_ALLOCATION);
	key_len = ft_strlen(key);
	i = 0;
	j = 0;
	while ((*env)[i] != NULL)
	{
		ptr = ft_strstr((*env)[i], key);
		if (ptr == NULL || ptr[key_len] != '=')
			array[j++] = (*env)[i];
		else
			free(ptr);
		i++;
	}
	return (free(*env), *env = array, ERR_NONE);
}
