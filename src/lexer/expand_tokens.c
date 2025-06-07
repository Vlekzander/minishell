/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 16:17:14 by apierret          #+#    #+#             */
/*   Updated: 2025/06/07 18:38:27 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "expand.h"
#include "lexer.h"
#include "utils.h"

static t_error	process_expand_env(t_list **node, t_token *token, char **env)
{
	char	*str;
	t_list	*list;
	t_error	error;

	if (node == NULL || token == NULL || env == NULL)
		return (ERR_IMPLEMENTATION);
	error = expand_env(&str, token->value, env, 0);
	if (error != ERR_NONE)
		return (error);
	if (str == token->value)
		return (ERR_NONE);
	if (ft_strlen(str) == 0)
		token->value[0] = '\0';
	else
	{
		error = split_lst(&list, str, ' ');
		if (error != ERR_NONE)
			return (free(str), error);
		error = insert_tkword_sublist(node, list);
		if (error != ERR_NONE)
			return (free(str), ft_lstclear(&list, free), error);
		ft_lstclear(&list, free);
	}
	return (free(str), ERR_NONE);
}

static t_error	process_expand_wildcard(t_list **node, t_token *token)
{
	t_list		*filtered;
	t_error		error;

	if (node == NULL || *node == NULL || token == NULL)
		return (ERR_IMPLEMENTATION);
	error = expand_wildcard(&filtered, token->value);
	if (error != ERR_NONE)
		return (error);
	if (filtered != NULL)
	{
		error = insert_tkword_sublist(node, filtered);
		if (error != ERR_NONE)
			return (error);
		ft_lstclear(&filtered, free);
	}
	return (ERR_NONE);
}

static t_error	apply_expand(t_list **tk_list, char **env, int is_env)
{
	t_list		*node;
	t_list		*next;
	t_token		*token;
	t_error		error;

	if (tk_list == NULL || env == NULL)
		return (ERR_IMPLEMENTATION);
	node = *tk_list;
	while (node != NULL)
	{
		next = node->next;
		token = node->content;
		if (token->type == TK_WORD)
		{
			if (is_env)
				error = process_expand_env(&node, token, env);
			else
				error = process_expand_wildcard(&node, token);
			if (error != ERR_NONE)
				return (error);
		}
		node = next;
	}
	return (ERR_NONE);
}

t_error	expand_tokens(t_list **tk_list, char **env)
{
	t_error		error;

	if (tk_list == NULL || env == NULL)
		return (ERR_IMPLEMENTATION);
	error = apply_expand(tk_list, env, 1);
	if (error != ERR_NONE)
		return (error);
	error = apply_expand(tk_list, env, 0);
	if (error != ERR_NONE)
		return (error);
	return (remove_empty_tokens(tk_list), ERR_NONE);
}
