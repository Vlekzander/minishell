/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_array.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 19:16:31 by apierret          #+#    #+#             */
/*   Updated: 2025/07/08 19:24:54 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "utils.h"

static int	compare(char *s1, char *s2, int (*cmp)(char c))
{
	int	value1;
	int	value2;
	int	i;

	if (s1 == NULL || s2 == NULL || cmp == NULL)
		return (0);
	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0')
	{
		value1 = cmp(s1[i]);
		value2 = cmp(s2[i]);
		if (value1 != value2)
			return (value1 - value2);
		i++;
	}
	return (cmp(s1[i]) - cmp(s2[i]));
}

void	sort_array(char **array, int (*cmp)(char c))
{
	int		i;
	int		j;
	char	*temp;

	if (array == NULL)
		return ;
	i = 0;
	while (array[i] != NULL)
	{
		j = i + 1;
		while (array[j] != NULL)
		{
			if (compare(array[i], array[j], cmp) > 0)
			{
				temp = array[i];
				array[i] = array[j];
				array[j] = temp;
			}
			j++;
		}
		i++;
	}
}
