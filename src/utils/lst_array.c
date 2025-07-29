/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_array.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:26:42 by apierret          #+#    #+#             */
/*   Updated: 2025/07/29 14:26:55 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

t_error	lst_array(char ***array, t_list *lst)
{
	char	**arr;
	size_t	size;
	size_t	i;

	if (lst == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	size = ft_lstsize(lst);
	arr = ft_calloc(size +1, sizeof(char *));
	if (arr == NULL)
		return (error(ERR_ALLOCATION, NULL));
	i = 0;
	while (lst != NULL)
	{
		arr[i] = ft_strdup(lst->content);
		if (arr[i] == NULL)
		{
			free_ddarray((void **) arr);
			return (error(ERR_ALLOCATION, NULL));
		}
		i++;
		lst = lst->next;
	}
	return (*array = arr, error(ERR_NONE, NULL));
}
