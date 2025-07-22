/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 13:54:11 by apierret          #+#    #+#             */
/*   Updated: 2025/07/15 00:43:36 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "expand.h"
#include "utils.h"

static t_error	proc_expand_env(char **output, t_hash_table *env, char **mask)
{
	t_list	*vars;
	char	*str;
	t_error	err;

	if (output == NULL || env == NULL || mask == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = extract_vars(&vars, *output, *mask, env);
	if (err.id != ERR_NONE)
		return (err);
	if (vars == NULL)
		return (error(ERR_NONE, NULL));
	err = substitute_vars(&str, *output, vars, 0);
	if (err.id != ERR_NONE)
		return (ft_lstclear(&vars, free), err);
	free(*output);
	*output = str;
	err = substitute_vars(&str, *mask, vars, 1);
	if (err.id != ERR_NONE)
		return (ft_lstclear(&vars, free), err);
	free(*mask);
	*mask = str;
	return (ft_lstclear(&vars, free), error(ERR_NONE, NULL));
}

static t_error	proc_expand_wildcard(char **output, char **mask)
{
	char	*base;
	char	*str;
	size_t	len;
	t_error	err;

	if (output == NULL || mask == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	base = *output;
	err = expand_wildcard(&str, base, *mask);
	if (err.id != ERR_NONE)
		return (err);
	if (str == base)
		return (error(ERR_NONE, NULL));
	len = ft_strlen(str);
	free(*mask);
	*mask = ft_calloc(len +1, sizeof(char));
	if (*mask == NULL)
		return (free(str), error(ERR_ALLOCATION, NULL));
	ft_memset(*mask, ' ', len);
	*output = str;
	return (free(base), error(ERR_NONE, NULL));
}

static void	insert_sublist(t_list *list, t_list *sublist)
{
	void	*temp;

	if (list == NULL || sublist == NULL)
		return ;
	temp = list->content;
	ft_lstadd_back(&sublist, list->next);
	list->next = sublist->next;
	list->content = sublist->content;
	free(sublist);
	free(temp);
}

static t_error	process_loop(t_list *node, char **mask, t_hash_table *env)
{
	t_list	*list;
	t_error	err;

	if (node == NULL || mask == NULL || *mask == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = proc_expand_env((char **) &node->content, env, mask);
	if (err.id != ERR_NONE)
		return (err);
	err = proc_expand_wildcard((char **) &node->content, mask);
	if (err.id != ERR_NONE)
		return (err);
	err = split_list_mask(&list, node->content, *mask, 0);
	if (err.id != ERR_NONE)
		return (err);
	if (list != NULL)
		insert_sublist(node, list);
	else
	{
		free(node->content);
		node->content = NULL;
	}
	return (error(ERR_NONE, NULL));
}

t_error	expand_list(t_list **lst, t_hash_table *env)
{
	t_list	*node;
	t_list	*next;
	char	*mask;
	t_error	err;

	if (lst == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	node = *lst;
	while (node != NULL)
	{
		next = node->next;
		err = prepare_mask(&mask, node->content, 0);
		if (err.id != ERR_NONE)
			return (err);
		err = process_loop(node, &mask, env);
		if (err.id != ERR_NONE)
			return (free(mask), err);
		free(mask);
		node = next;
	}
	lst_remove(lst, ptr_null, NULL);
	if (*lst == NULL)
		return (error(ERR_NO_EXEC, NULL));
	return (error(ERR_NONE, NULL));
}
