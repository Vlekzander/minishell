/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:16:09 by apierret          #+#    #+#             */
/*   Updated: 2025/05/24 23:33:23 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "env.h"
#include "utils.h"

t_error	get_var(char **str, char **env, char *key)
{
	char	*ptr;
	size_t	size;

	if (str == NULL || env == NULL || key == NULL)
		return (ERR_IMPLEMENTATION);
	ptr = NULL;
	size = ft_strlen(key);
	while (*env != NULL)
	{
		ptr = ft_strstr(*env, key);
		if (ptr != NULL && ptr[size] == '=')
			return (*str = ptr + size + 1, ERR_NONE);
		env++;
	}
	return (*str = NULL, ERR_NONE);
}
