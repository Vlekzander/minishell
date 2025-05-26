/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:16:14 by apierret          #+#    #+#             */
/*   Updated: 2025/05/26 18:12:58 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "env.h"
#include "libft.h"
#include "utils.h"

static t_error	replace_var(char **current_str, char *key, char *value)
{
	char	*str;
	size_t	key_len;
	size_t	value_len;

	if (current_str == NULL || key == NULL || value == NULL)
		return (ERR_IMPLEMENTATION);
	key_len = ft_strlen(key);
	value_len = ft_strlen(value);
	str = ft_calloc(key_len + value_len + 2, sizeof(char));
	if (str == NULL)
		return (ERR_ALLOCATION);
	str[0] = '\0';
	ft_strlcat(str, key, key_len + value_len + 2);
	ft_strlcat(str, "=", key_len + value_len + 2);
	ft_strlcat(str, value, key_len + value_len + 2);
	return (free(*current_str), *current_str = str, ERR_NONE);
}

static t_error	add_var(char ***array, char *key, char *value)
{
	char	*str;
	size_t	size;
	t_error	error;

	if (array == NULL || key == NULL || value == NULL)
		return (ERR_IMPLEMENTATION);
	size = ft_strlen(key);
	size += 1;
	size += ft_strlen(value);
	str = ft_calloc(size +1, sizeof(char));
	if (str == NULL)
		return (ERR_ALLOCATION);
	ft_strlcat(str, key, size +1);
	ft_strlcat(str, "=", size +1);
	ft_strlcat(str, value, size +1);
	error = str_array_push(array, str);
	return (free(str), error);
}

t_error	set_var(char ***env, char *key, char *value)
{
	char	*ptr;
	size_t	key_len;
	size_t	i;
	t_error	error;

	if (env == NULL || *env == NULL || key == NULL || value == NULL)
		return (ERR_IMPLEMENTATION);
	ptr = NULL;
	key_len = ft_strlen(key);
	i = 0;
	while ((*env)[i] != NULL && ptr == NULL)
	{
		ptr = ft_strstr((*env)[i], key);
		if (ptr != NULL && ptr[key_len] != '=')
			ptr = NULL;
		i++;
	}
	if (ptr == NULL)
		error = add_var(env, key, value);
	else
		error = replace_var(&(*env)[i - 1], key, value);
	if (error != ERR_NONE)
		return (error);
	return (ERR_NONE);
}
