/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 12:18:09 by apierret          #+#    #+#             */
/*   Updated: 2025/06/03 16:18:27 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "lexer_internal.h"
#include "utils.h"

static t_error	process_exp_env(t_list	**node, t_token *token, t_list *lst_var,
									char **env)
{
	t_list	*list;
	t_error	error;
	char	*str;

	if (node == NULL || token == NULL || env == NULL)
		return (ERR_IMPLEMENTATION);
	error = override_vars(&str, token->value, lst_var, env);
	if (error != ERR_NONE)
		return (ft_lstclear(&lst_var, (void *) free_vref), error);
	ft_lstclear(&lst_var, (void *) free_vref);
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

static t_error	expand_env(t_list **node, t_token *token, char **env)
{
	t_vref	*var;
	char	*str;
	t_list	*vrefs;
	t_list	*node_var;
	t_error	error;

	if (node == NULL || *node == NULL || token == NULL || env == NULL)
		return (ERR_IMPLEMENTATION);
	vrefs = NULL;
	str = token->value;
	while (1)
	{
		error = extract_var(&var, str);
		if (error != ERR_NONE)
			return (free_vref(var), error);
		if (var->str == NULL)
			break ;
		node_var = ft_lstnew(var);
		if (node_var == NULL)
			return (ft_lstclear(&vrefs, (void *) free_vref),
				free_vref(var), ERR_ALLOCATION);
		ft_lstadd_back(&vrefs, node_var);
		str = token->value + var->index + ft_strlen(var->str);
	}
	return (free_vref(var), process_exp_env(node, token, vrefs, env), ERR_NONE);
}

static t_error	expand_wildcard(t_list **node, t_token *token)
{
	t_pattern	*pattern;
	t_list		*files;
	t_list		*filtered;
	t_error		error;

	if (node == NULL || *node == NULL || token == NULL)
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
			error = insert_tkword_sublist(node, filtered);
		ft_lstclear(&files, free);
		ft_lstclear(&filtered, free);
	}
	free_pattern(pattern);
	return (error);
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
				error = expand_env(&node, token, env);
			else
				error = expand_wildcard(&node, token);
			if (error != ERR_NONE)
				return (error);
		}
		node = next;
	}
	return (ERR_NONE);
}

t_error	expand(t_list **tk_list, char **env)
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
