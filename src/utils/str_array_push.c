/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_array_push.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 14:03:58 by apierret          #+#    #+#             */
/*   Updated: 2025/05/22 19:16:40 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "utils.h"

t_error	str_array_push(char ***array, char *element)
{
	char	**new_array;
	size_t	size;
	size_t	i;

	if (array == NULL || element == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	size = 0;
	while (*array != NULL && (*array)[size] != NULL)
		size++;
	new_array = ft_calloc(size + 2, sizeof(char *));
	if (new_array == NULL)
		return (error(ERR_ALLOCATION, NULL));
	i = 0;
	while (*array != NULL && i < size)
	{
		new_array[i] = (*array)[i];
		i++;
	}
	new_array[i] = ft_strdup(element);
	if (new_array[i] == NULL)
		return (free(new_array), error(ERR_ALLOCATION, NULL));
	return (free(*array), *array = new_array, error(ERR_NONE, NULL));
}
