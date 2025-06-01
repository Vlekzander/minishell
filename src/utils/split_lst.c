/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 23:37:37 by apierret          #+#    #+#             */
/*   Updated: 2025/06/02 00:23:31 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "error.h"
#include "libft.h"
#include "utils.h"

t_error	split_lst(t_list **lst, char *str, char c)
{
	t_list	*list;
	t_list	*node;
	size_t	size;
	char	*ptr;
	char	*content;

	if (lst == NULL || str == NULL)
		return (ERR_IMPLEMENTATION);
	list = NULL;
	if (*str == '\0')
	{
		content = ft_calloc(1, sizeof(char));
		if (content == NULL)
			return (ERR_ALLOCATION);
		node = ft_lstnew(content);
		if (node == NULL)
			return (free(content), ft_lstclear(&list, free), ERR_ALLOCATION);
		ft_lstadd_back(&list, node);
	}
	while (*str != '\0')
	{
		ptr = ft_strchr(str, c);
		if (ptr == NULL)
			ptr = str + ft_strlen(str);
		size = ptr - str;
		if (size > 0)
		{
			content = ft_strndup(str, size);
			if (content == NULL)
				return (ft_lstclear(&list, free), ERR_ALLOCATION);
			node = ft_lstnew(content);
			if (node == NULL)
				return (free(content), ft_lstclear(&list, free), ERR_ALLOCATION);
			ft_lstadd_back(&list, node);
		}
		str = ptr;
		if (*ptr != '\0')
			str++;
	}
	return (*lst = list, ERR_NONE);
}
