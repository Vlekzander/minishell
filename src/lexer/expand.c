/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 12:18:09 by apierret          #+#    #+#             */
/*   Updated: 2025/06/02 00:27:05 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "env.h"
#include "lexer.h"
#include "lexer_internal.h"
#include "utils.h"

static t_error	create_token_sublift(t_list **sublist, t_list *strings)
{
	t_list	*lst;
	t_list	*node;
	t_token	*token;

	if (sublist == NULL || strings == NULL)
		return (ERR_IMPLEMENTATION);
	lst = NULL;
	*sublist = NULL;
	while (strings != NULL)
	{
		token = create_token(TK_WORD, strings->content);
		if (token == NULL)
			return (ft_lstclear(&lst, (void *) free_token), ERR_ALLOCATION);
		node = ft_lstnew(token);
		if (node == NULL)
			return (ft_lstclear(&lst, (void *) free_token), free_token(token),
				ERR_ALLOCATION);
		ft_lstadd_back(&lst, node);
		strings = strings->next;
	}
	return (*sublist = lst, ERR_NONE);
}

static t_error	insert_sublist(t_list **node, t_list *strings)
{
	t_list	*sublist;
	t_error	error;

	if (node == NULL || strings == NULL)
		return (ERR_IMPLEMENTATION);
	error = create_token_sublift(&sublist, strings);
	if (error != ERR_NONE)
		return (error);
	free_token((*node)->content);
	ft_lstadd_back(&sublist, (*node)->next);
	(*node)->content = sublist->content;
	(*node)->next = sublist->next;
	ft_lstdelone(sublist, NULL);
	return (ERR_NONE);
}

static t_error	over_var(char **output, char *base, t_varpos *var, char **env)
{
	char	*str_one;
	char	*str_two;
	char	*var_content;
	t_error	error;

	if (output == NULL || base == NULL || var == NULL || var->str == NULL
		|| env == NULL)
		return (ERR_IMPLEMENTATION);
	str_one = ft_strndup(base, var->index);
	if (str_one == NULL)
		return (ERR_ALLOCATION);
	error = get_var(&var_content, env, var->str +1);
	if (error != ERR_NONE)
		return (free(str_one), error);

	str_two = ft_strjoin(str_one, var_content);
	if (str_two == NULL)
		return (free(str_one), free(var_content), ERR_ALLOCATION);
	free(str_one);
	str_one = ft_strjoin(str_two, base + var->index + ft_strlen(var->str));
	if (str_one == NULL)
		return (ERR_ALLOCATION);
	free(str_two);
	return (*output = str_one, ERR_NONE);
}

static t_error	expand_env(t_list **node, t_token *token, char **env)
{
	t_varpos	*var;
	char		*str;
	char		*temp;
	t_error		error;
	t_list		*lst;

	if (node == NULL || *node == NULL || token == NULL || env == NULL)
		return (ERR_IMPLEMENTATION);
	error = ERR_NONE;
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
			break;
		error = over_var(&str, temp, var, env);
		if (error != ERR_NONE)
			return (free_varpos(var), error);
	}
	free_varpos(var);
	if (str != token->value)
	{
		error = split_lst(&lst, str, ' ');
		if (error != ERR_NONE)
			return (error);
		error = insert_sublist(node, lst);
	}
	return (error);
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
			error = insert_sublist(node, filtered);
		ft_lstclear(&files, free);
		ft_lstclear(&filtered, NULL);
	}
	free_pattern(pattern);
	return (error);
}

t_error	remove_empty_node(t_list **tk_list)
{
	t_list	*current;
	t_list	*prev;
	t_list	*tmp;
	t_token	*token;

	if (tk_list == NULL || *tk_list == NULL)
		return (ERR_IMPLEMENTATION);
	current = *tk_list;
	prev = NULL;
	while (current != NULL)
	{
		token = (t_token *) current->content;
		if (ft_strlen(token->value) == 0)
		{
			tmp = current->next;
			if (prev == NULL)
				*tk_list = tmp;
			else
				prev->next = tmp;
			ft_lstclear(&current, free);
			current = tmp;
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
	return (ERR_NONE);
}

t_error	expand(t_list **tk_list, char **env)
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
			error = expand_env(&node, token, env);
			if (error != ERR_NONE)
				return (error);
			error = expand_wildcard(&node, token);
			if (error != ERR_NONE)
				return (error);
		}
		node = next;
	}
	remove_empty_node(tk_list);
	return (ERR_NONE);
}
