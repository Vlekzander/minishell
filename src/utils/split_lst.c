/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 23:37:37 by apierret          #+#    #+#             */
/*   Updated: 2025/06/02 14:17:26 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "error.h"
#include "libft.h"
#include "utils.h"

static t_error	new_node(t_list **node, char *input, size_t size)
{
	char	*content;
	t_list	*ptr;

	if (node == NULL || input == NULL)
		return (ERR_IMPLEMENTATION);
	content = ft_strndup(input, size);
	if (content == NULL)
		return (ERR_ALLOCATION);
	ptr = ft_lstnew(content);
	if (ptr == NULL)
		return (free(content), ERR_ALLOCATION);
	return (*node = ptr, ERR_NONE);
}

t_error	split_lst(t_list **lst, char *input, char c)
{
	t_list	*list;
	t_list	*node;
	char	*ptr;
	t_error	error;

	if (lst == NULL || input == NULL)
		return (ERR_IMPLEMENTATION);
	list = NULL;
	while (*input != '\0')
	{
		ptr = ft_strchr(input, c);
		if (ptr == NULL)
			ptr = input + ft_strlen(input);
		if (ptr - input > 0)
		{
			error = new_node(&node, input, ptr - input);
			if (error != ERR_NONE)
				return (ft_lstclear(&list, free), error);
			ft_lstadd_back(&list, node);
		}
		input = ptr;
		if (*ptr != '\0')
			input++;
	}
	return (*lst = list, ERR_NONE);
}
