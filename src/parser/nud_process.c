/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nud_process.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 12:24:12 by apierret          #+#    #+#             */
/*   Updated: 2025/06/19 13:54:41 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "data.h"
#include "error.h"
#include "parser.h"
#include "utils.h"

static t_error	nud_word(t_ast **ast, t_token *token)
{
	t_ast	*node;
	t_list	*arg;
	char	*str;

	if (ast == NULL || token == NULL)
		return (ERR_IMPLEMENTATION);
	node = create_ast(NODE_COMMAND);
	if (node == NULL)
		return (ERR_ALLOCATION);
	str = ft_strdup(token->value);
	if (str == NULL)
		return (ERR_ALLOCATION);
	arg = ft_lstnew(str);
	if (arg == NULL)
		return (free(str), ERR_ALLOCATION);
	ft_lstadd_back(&node->command_args, arg);
	if (*ast != NULL)
	{
		node->redirs = (*ast)->redirs;
		(*ast)->redirs = NULL;
		free_ast(*ast);
	}
	return (*ast = node, ERR_NONE);
}

static t_error	nud_group(t_ast **ast, t_list **tk_lst, t_hash_table *env)
{
	t_ast	*node;
	t_error	error;
	t_token	*next;

	if (*ast != NULL)
		return (ERR_SYNTAX);
	node = create_ast(NODE_GROUP);
	if (node == NULL)
		return (ERR_ALLOCATION);
	error = parse_expression(&node->group, tk_lst, env, -1);
	next = peek_front(tk_lst, 0);
	if (node->group != NULL && next != NULL && next->type == TK_P_CLOSE)
		peek_front(tk_lst, 1);
	else
		error = ERR_SYNTAX;
	if (error != ERR_NONE)
		return (free_ast(node), error);
	return (*ast = node, ERR_NONE);
}

t_error	nud(t_ast **ast, t_list **tk_lst, t_hash_table *env, t_token *token)
{
	t_error	error;

	if (ast == NULL || token == NULL)
		return (ERR_IMPLEMENTATION);
	if (token->type == TK_WORD)
	{
		error = nud_word(ast, token);
		if (error != ERR_NONE)
			return (error);
	}
	else if (token->type == TK_P_OPEN)
	{
		error = nud_group(ast, tk_lst, env);
		if (error != ERR_NONE)
			return (error);
	}
	else
		return (ERR_SYNTAX);
	return (ERR_NONE);
}
