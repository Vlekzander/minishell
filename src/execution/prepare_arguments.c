/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_arguments.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 11:56:09 by apierret          #+#    #+#             */
/*   Updated: 2025/07/03 12:01:33 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "libft.h"
#include "utils.h"

static t_error	lst_to_arr(char ***array, t_list *lst)
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

t_error	prepare_arguments(char ***args, t_list *cmd_args)
{
	char	**arr;
	char	*ptr;
	t_error	err;

	if (args == NULL || cmd_args == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = lst_to_arr(&arr, cmd_args);
	if (err.id != ERR_NONE)
		return (err);
	ptr = ft_strrchr(arr[0], '/');
	if (ptr != NULL)
	{
		if (ptr[1] == '\0')
		{
			free_ddarray((void **) arr);
			return (error(ERR_IS_DIRECTORY, cmd_args->content));
		}
		ft_memmove(arr[0], ptr +1, ft_strlen(ptr));
	}
	return (*args = arr, error(ERR_NONE, NULL));
}
