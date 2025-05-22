/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 12:18:09 by apierret          #+#    #+#             */
/*   Updated: 2025/05/22 14:15:25 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "lexer.h"
#include "utils.h"

static t_error	tokens_sublist(t_list **sublist, t_list *filtered)
{
	t_list	*lst;
	t_list	*node;
	t_token	*token;

	if (sublist == NULL || filtered == NULL)
		return (ERR_IMPLEMENTATION);
	lst = NULL;
	*sublist = NULL;
	while (filtered != NULL)
	{
		token = create_token(TK_WORD, filtered->content);
		if (token == NULL)
			return (ft_lstclear(&lst, (void *) free_token), ERR_ALLOCATION);
		node = ft_lstnew(token);
		if (node == NULL)
			return (ft_lstclear(&lst, (void *) free_token), free_token(token),
				ERR_ALLOCATION);
		ft_lstadd_back(&lst, node);
		filtered = filtered->next;
	}
	return (*sublist = lst, ERR_NONE);
}

static t_error	insert_filtered_files(t_list **node, t_list *filtered)
{
	t_list	*sublist;
	t_error	error;

	if (node == NULL || filtered == NULL)
		return (ERR_IMPLEMENTATION);
	error = tokens_sublist(&sublist, filtered);
	if (error != ERR_NONE)
		return (error);
	free_token((*node)->content);
	ft_lstadd_back(&sublist, (*node)->next);
	(*node)->content = sublist->content;
	(*node)->next = sublist->next;
	ft_lstdelone(sublist, NULL);
	return (ERR_NONE);
}

static t_error	expand_wildcard(t_list **node, t_token *token)
{
	t_pattern	*pattern;
	t_list		*files;
	t_list		*filtered;
	t_error		error;

	if (node == NULL || token == NULL)
		return (ERR_IMPLEMENTATION);
	error = extract_pattern(&pattern, token->value);
	if (error != ERR_NONE)
		return (error);
	if (pattern != NULL)
	{
		error = scan_dir(&files, ".");
		if (error != ERR_NONE)
			return (free_pattern(pattern), error);
		error = globbing(&filtered, files, pattern);
		if (error != ERR_NONE)
			return (free_pattern(pattern), ft_lstclear(&files, free), error);
		if (filtered != NULL)
			error = insert_filtered_files(node, filtered);
		ft_lstclear(&files, free);
		ft_lstclear(&filtered, NULL);
	}
	free_pattern(pattern);
	return (error);
}

t_error	expand(t_list **tokens)
{
	t_list		*node;
	t_list		*next;
	t_token		*token;
	t_error		error;

	if (tokens == NULL)
		return (ERR_IMPLEMENTATION);
	node = *tokens;
	while (node != NULL)
	{
		next = node->next;
		token = node->content;
		error = expand_wildcard(&node, token);
		if (error != ERR_NONE)
			return (error);
		node = next;
	}
	return (ERR_NONE);
}
