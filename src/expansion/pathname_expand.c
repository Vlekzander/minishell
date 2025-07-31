/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathname_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:03:13 by apierret          #+#    #+#             */
/*   Updated: 2025/07/31 14:52:50 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "expansion.h"
#include "utils.h"

static size_t	calculate_sublist_len(t_list *sublist)
{
	size_t	len;

	if (sublist == NULL)
		return (0);
	len = 0;
	while (sublist != NULL)
	{
		len += ft_strlen(sublist->content) + 1;
		sublist = sublist->next;
	}
	return (len);
}

static t_error	pathname_expand_handle(t_list **lst, char *str,
					t_strbuilder *sb, size_t i)
{
	t_error	err;
	char	*temp;
	t_list	*sublist;

	if (lst == NULL || str == NULL || sb == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = pathname_expansion(&sublist, str, sb->buffer + i, NULL);
	if (err.id != ERR_NONE)
		return (err);
	if (sublist != NULL)
	{
		temp = ft_strdup(sb->buffer + i + ft_strlen(str));
		if (temp == NULL)
			return (error(ERR_ALLOCATION, NULL));
		sb->length = i;
		if (!strbuilder_append_mchar(sb, ' ', calculate_sublist_len(sublist)))
			return (free(temp), error(ERR_ALLOCATION, NULL));
		if (!strbuilder_append(sb, temp))
			return (free(temp), error(ERR_ALLOCATION, NULL));
		free(temp);
	}
	return (*lst = sublist, error(ERR_NONE, NULL));
}

static t_error	process_pathname_expand(t_list **lst, t_strbuilder *sb)
{
	t_list			*sublist;
	t_list			*node;
	t_list			*next;
	size_t			i;
	t_error			err;

	i = 0;
	node = *lst;
	while (node != NULL)
	{
		next = node->next;
		err = pathname_expand_handle(&sublist, node->content, sb, i);
		if (err.id != ERR_NONE)
			return (err);
		i += calculate_sublist_len(sublist);
		if (sublist != NULL)
			insert_sublist(node, sublist);
		else
			i += ft_strlen(node->content) + 1;
		node = next;
	}
	return (error(ERR_NONE, NULL));
}

t_error	pathname_expand_list(t_list **lst, char **mask)
{
	t_strbuilder	*sb;
	t_error			err;

	if (lst == NULL || mask == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (*lst == NULL)
		return (error(ERR_NONE, NULL));
	sb = create_strbuilder(ft_strlen(*mask) + 1);
	if (sb == NULL)
		return (error(ERR_ALLOCATION, NULL));
	if (!strbuilder_append(sb, *mask))
		return (free_strbuilder(sb), error(ERR_ALLOCATION, NULL));
	err = process_pathname_expand(lst, sb);
	if (err.id != ERR_NONE)
		return (free_strbuilder(sb), err);
	free(*mask);
	*mask = ft_strdup(sb->buffer);
	return (free_strbuilder(sb), error(ERR_NONE, NULL));
}
