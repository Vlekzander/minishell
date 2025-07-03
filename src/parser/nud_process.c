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
		return (error(ERR_IMPLEMENTATION, NULL));
	node = create_ast(NODE_COMMAND);
	if (node == NULL)
		return (error(ERR_ALLOCATION, NULL));
	str = ft_strdup(token->value);
	if (str == NULL)
		return (error(ERR_ALLOCATION, NULL));
	arg = ft_lstnew(str);
	if (arg == NULL)
		return (free(str), error(ERR_ALLOCATION, NULL));
	ft_lstadd_back(&node->command_args, arg);
	if (*ast != NULL)
	{
		node->redirs = (*ast)->redirs;
		(*ast)->redirs = NULL;
		free_ast(*ast);
	}
	return (*ast = node, error(ERR_NONE, NULL));
}

static t_error	nud_group(t_ast **ast, t_list **tk_lst, t_hash_table *env)
{
	t_ast	*node;
	t_error	err;
	t_token	*next;

	if (*ast != NULL)
		return (error(ERR_SYNTAX, NULL));
	node = create_ast(NODE_GROUP);
	if (node == NULL)
		return (error(ERR_ALLOCATION, NULL));
	err = parse_expression(&node->group, tk_lst, env, -1);
	next = peek_front(tk_lst, 0);
	if (node->group != NULL && next != NULL && next->type == TK_P_CLOSE)
		peek_front(tk_lst, 1);
	else
		err = error(ERR_SYNTAX, NULL);
	if (err.id != ERR_NONE)
		return (free_ast(node), err);
	return (*ast = node, error(ERR_NONE, NULL));
}

t_error	nud(t_ast **ast, t_list **tk_lst, t_hash_table *env, t_token *token)
{
	t_error	err;

	if (ast == NULL || token == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (token->type == TK_WORD)
	{
		err = nud_word(ast, token);
		if (err.id != ERR_NONE)
			return (err);
	}
	else if (token->type == TK_P_OPEN)
	{
		err = nud_group(ast, tk_lst, env);
		if (err.id != ERR_NONE)
			return (err);
	}
	else
		return (error(ERR_SYNTAX, NULL));
	return (error(ERR_NONE, NULL));
}
