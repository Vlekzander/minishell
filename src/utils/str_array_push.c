/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_array_push.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 14:03:58 by apierret          #+#    #+#             */
/*   Updated: 2025/04/03 16:03:45 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "utils.h"

char	**str_array_push(char **array, char *element)
{
	char	**new_array;
	size_t	size;
	size_t	i;

	size = 0;
	while (array != NULL && array[size] != NULL)
		size++;
	new_array = ft_calloc(size +2, sizeof(char *));
	if (new_array == NULL)
		return (NULL);
	i = 0;
	while (array != NULL && i < size)
	{
		new_array[i] = array[i];
		i++;
	}
	free(array);
	new_array[i] = ft_strdup(element);
	return (new_array);
}
