/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:31:23 by apierret          #+#    #+#             */
/*   Updated: 2025/07/07 16:20:12 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "expand.h"
#include "utils.h"

static t_error	process_expand_env(t_list *node, t_hash_table *env)
{
	char	*base;
	char	*str;
	t_list	*list;
	t_error	err;

	if (node == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	base = node->content;
	err = expand_env(&str, base, env, 0);
	if (err.id != ERR_NONE)
		return (err);
	if (str == base)
		return (error(ERR_NONE, NULL));
	if (ft_strlen(str) != 0)
	{
		err = split_lst(&list, str, ' ');
		if (err.id != ERR_NONE)
			return (free(str), err);
		err = insert_sublist(&node, list);
		if (err.id != ERR_NONE)
			return (free(str), ft_lstclear(&list, free), err);
	}
	base[0] = '\0';
	return (free(str), error(ERR_NONE, NULL));
}

static t_error	process_expand_wildcard(t_list *node)
{
	char	*base;
	char	*str;
	t_list	*list;
	t_error	err;

	if (node == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	base = node->content;
	err = expand_wildcard(&str, base);
	if (err.id != ERR_NONE)
		return (err);
	if (str == base)
		return (error(ERR_NONE, NULL));
	err = split_lst(&list, str, ' ');
	if (err.id != ERR_NONE)
		return (free(str), err);
	err = insert_sublist(&node, list);
	if (err.id != ERR_NONE)
		return (free(str), ft_lstclear(&list, free), err);
	base[0] = '\0';
	return (free(str), error(ERR_NONE, NULL));
}

t_error	expand_list(t_list **lst, t_hash_table *env)
{
	t_list	*node;
	t_error	err;

	if (lst == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	node = *lst;
	while (node != NULL)
	{
		err = process_expand_env(node, env);
		if (err.id != ERR_NONE)
			return (err);
		err = process_expand_wildcard(node);
		if (err.id != ERR_NONE)
			return (err);
		node = node->next;
	}
	lst_remove(lst, str_empty, free);
	if (*lst == NULL)
		return (error(ERR_CMD_EMPTY, NULL));
	return (error(ERR_NONE, NULL));
}
