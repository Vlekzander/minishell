/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathname_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:03:13 by apierret          #+#    #+#             */
/*   Updated: 2025/07/31 15:33:38 by apierret         ###   ########.fr       */
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

static t_error	pathname_expand_handle(t_list **lst, char *str, t_sb_index *sbi,
					t_hash_table *env)
{
	t_error	err;
	char	*temp;
	t_list	*sublist;

	if (lst == NULL || str == NULL || sbi == NULL || sbi->sb == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = pathname_expansion(&sublist, str, sbi->sb->buffer + sbi->index,
			htable_get(env, "GLOBIGNORE"));
	if (err.id != ERR_NONE)
		return (err);
	if (sublist != NULL)
	{
		temp = ft_strdup(sbi->sb->buffer + sbi->index + ft_strlen(str));
		if (temp == NULL)
			return (error(ERR_ALLOCATION, NULL));
		sbi->sb->length = sbi->index;
		if (!strbuilder_append_mchar(sbi->sb, ' ',
				calculate_sublist_len(sublist)))
			return (free(temp), error(ERR_ALLOCATION, NULL));
		if (!strbuilder_append(sbi->sb, temp))
			return (free(temp), error(ERR_ALLOCATION, NULL));
		free(temp);
	}
	return (*lst = sublist, error(ERR_NONE, NULL));
}

static t_error	process_pathname_expand(t_list **lst, t_sb_index *sbi,
					t_hash_table *env)
{
	t_list			*sublist;
	t_list			*node;
	t_list			*next;
	t_error			err;

	sbi->index = 0;
	node = *lst;
	while (node != NULL)
	{
		next = node->next;
		err = pathname_expand_handle(&sublist, node->content, sbi, env);
		if (err.id != ERR_NONE)
			return (err);
		sbi->index += calculate_sublist_len(sublist);
		if (sublist != NULL)
			insert_sublist(node, sublist);
		else
			sbi->index += ft_strlen(node->content) + 1;
		node = next;
	}
	return (error(ERR_NONE, NULL));
}

t_error	pathname_expand_list(t_list **lst, char **mask, t_hash_table *env)
{
	t_sb_index		sbi;
	t_error			err;

	if (lst == NULL || mask == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (*lst == NULL)
		return (error(ERR_NONE, NULL));
	sbi.sb = create_strbuilder(ft_strlen(*mask) + 1);
	if (sbi.sb == NULL)
		return (error(ERR_ALLOCATION, NULL));
	if (!strbuilder_append(sbi.sb, *mask))
		return (free_strbuilder(sbi.sb), error(ERR_ALLOCATION, NULL));
	err = process_pathname_expand(lst, &sbi, env);
	if (err.id != ERR_NONE)
		return (free_strbuilder(sbi.sb), err);
	free(*mask);
	*mask = ft_strdup(sbi.sb->buffer);
	return (free_strbuilder(sbi.sb), error(ERR_NONE, NULL));
}
