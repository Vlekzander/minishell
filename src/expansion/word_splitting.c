/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_splitting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 15:51:36 by apierret          #+#    #+#             */
/*   Updated: 2025/07/31 17:50:07 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "env.h"
#include "expansion.h"

static t_error	new_node(t_list **node, char *str)
{
	char	*content;
	t_list	*ptr;

	if (node == NULL || str == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	content = ft_strdup(str);
	if (content == NULL)
		return (error(ERR_ALLOCATION, NULL));
	ptr = ft_lstnew(content);
	if (ptr == NULL)
		return (free(content), error(ERR_ALLOCATION, NULL));
	return (*node = ptr, error(ERR_NONE, NULL));
}

static t_error	word_to_lst(t_list **lst, t_strbuilder *sb, char **mask)
{
	t_list	*node;
	t_error	err;

	if (lst == NULL || sb == NULL || mask == NULL || *mask == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (sb->length == 0)
		ft_memmove(*mask, (*mask) + 1, ft_strlen(*mask + 1) + 1);
	else
	{
		err = new_node(&node, sb->buffer);
		if (err.id != ERR_NONE)
			return (err);
		ft_lstadd_back(lst, node);
		(*mask) += sb->length + 1;
	}
	return (sb->length = 0, error(ERR_NONE, NULL));
}

static	t_error	prepare_splitting(t_list **lst, t_strbuilder **sb, char **ifs,
					size_t len)
{
	if (lst == NULL || sb == NULL || ifs == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	*lst = NULL;
	*sb = create_strbuilder(len +1);
	if (*sb == NULL)
		return (error(ERR_ALLOCATION, NULL));
	if (*ifs == NULL)
		*ifs = " \t\n";
	return (error(ERR_NONE, NULL));
}

t_error	word_splitting(t_list **lst, char *str, char *mask, char *ifs)
{
	t_list			*list;
	t_strbuilder	*sb;
	t_error			err;

	if (lst == NULL || str == NULL || mask == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = prepare_splitting(&list, &sb, &ifs, ft_strlen(str));
	if (err.id != ERR_NONE)
		return (err);
	while (*str != '\0')
	{
		if (mask[sb->length] == ' ' && ft_strchr(ifs, *str) != NULL)
			err = word_to_lst(&list, sb, &mask);
		else if (!strbuilder_append_char(sb, *str))
			err = error(ERR_ALLOCATION, NULL);
		if (err.id != ERR_NONE)
			return (free_strbuilder(sb), ft_lstclear(&list, free), err);
		str++;
	}
	err = word_to_lst(&list, sb, &mask);
	if (err.id != ERR_NONE)
		return (free_strbuilder(sb), ft_lstclear(&list, free), err);
	return (free_strbuilder(sb), *lst = list, error(ERR_NONE, NULL));
}
