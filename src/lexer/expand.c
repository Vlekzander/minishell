/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 12:18:09 by apierret          #+#    #+#             */
/*   Updated: 2025/06/02 16:28:42 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "lexer_internal.h"
#include "utils.h"

static t_error	apply_env_expand(t_list	**node, t_token *token, char *expanded)
{
	t_list	*list;
	t_error	error;

	if (node == NULL || token == NULL || expanded == NULL)
		return (ERR_IMPLEMENTATION);
	if (expanded != token->value)
	{
		if (ft_strlen(expanded) == 0)
			token->value[0] = '\0';
		else
		{
			error = split_lst(&list, expanded, ' ');
			if (error != ERR_NONE)
				return (error);
			error = insert_tkword_sublist(node, list);
			ft_lstclear(&list, free);
			if (error != ERR_NONE)
				return (error);
		}
		free(expanded);
	}
	return (ERR_NONE);
}

static t_error	expand_env(t_list **node, t_token *token, char **env)
{
	t_varpos	*var;
	char		*str;
	char		*temp;
	t_error		error;

	if (node == NULL || *node == NULL || token == NULL || env == NULL)
		return (ERR_IMPLEMENTATION);
	var = NULL;
	str = token->value;
	while (var == NULL || var->str != NULL)
	{
		free_varpos(var);
		temp = str;
		error = extract_var(&var, temp);
		if (error != ERR_NONE)
			return (free_varpos(var), error);
		if (var->str == NULL)
			break ;
		error = override_var(&str, temp, var, env);
		if (error != ERR_NONE)
			return (free_varpos(var), error);
		if (temp != token->value)
			free(temp);
	}
	return (free_varpos(var), apply_env_expand(node, token, str), ERR_NONE);
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
