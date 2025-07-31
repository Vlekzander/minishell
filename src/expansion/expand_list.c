/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 16:02:55 by apierret          #+#    #+#             */
/*   Updated: 2025/07/31 13:50:14 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "expansion.h"
#include "utils.h"

static t_error	sublist_quote_removal(t_list *sublist, char *mask)
{
	t_error	err;

	if (sublist == NULL || mask == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	while (sublist != NULL)
	{
		err = quote_removal(sublist->content, mask);
		if (err.id != ERR_NONE)
			return (err);
		mask += ft_strlen(sublist->content) + 1;
		sublist = sublist->next;
	}
	return (error(ERR_NONE, NULL));
}

static t_error	process_expand(t_list **lst, char **str, char **mask,
					t_hash_table *env)
{
	t_list	*sublist;
	t_error	err;

	if (lst == NULL || str == NULL || *str == NULL || mask == NULL
		|| env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = variable_expansion(str, mask, env);
	if (err.id != ERR_NONE)
		return (err);
	err = word_splitting(&sublist, *str, *mask, htable_get(env, "IFS"));
	if (err.id != ERR_NONE)
		return (err);
	err = pathname_expand_list(&sublist, mask);
	if (err.id != ERR_NONE)
		return (ft_lstclear(&sublist, free), err);
	err = sublist_quote_removal(sublist, *mask);
	if (err.id != ERR_NONE)
		return (ft_lstclear(&sublist, free), err);
	return (*lst = sublist, error(ERR_NONE, NULL));
}

t_error	expand_list(t_list **lst, t_hash_table *env)
{
	t_list	*node;
	t_list	*next;
	t_list	*sublist;
	char	*mask;
	t_error	err;

	if (lst == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	node = *lst;
	while (node != NULL)
	{
		next = node->next;
		err = create_mask(&mask, node->content);
		if (err.id != ERR_NONE)
			return (err);
		err = process_expand(&sublist, (char **) &node->content, &mask, env);
		if (err.id != ERR_NONE)
			return (free(mask), err);
		free(mask);
		insert_sublist(node, sublist);
		node = next;
	}
	return (error(ERR_NONE, NULL));
}
