/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   led_process.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:41:28 by apierret          #+#    #+#             */
/*   Updated: 2025/06/21 16:07:09 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "data.h"
#include "error.h"
#include "parser.h"
#include "utils.h"

static t_error	led_word(t_ast **ast, t_token *token)
{
	t_ast	*node;
	t_list	*arg;
	char	*str;

	if (ast == NULL || token == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if ((*ast)->type == NODE_COMMAND)
		node = (*ast);
	else if ((*ast)->type == NODE_PIPELINE)
		node = (t_ast *) ft_lstlast((*ast)->pipeline)->content;
	else
		return (error(ERR_SYNTAX, NULL));
	str = ft_strdup(token->value);
	if (str == NULL)
		return (error(ERR_ALLOCATION, NULL));
	arg = ft_lstnew(str);
	if (arg == NULL)
		return (free(str), error(ERR_ALLOCATION, NULL));
	ft_lstadd_back(&node->command_args, arg);
	return (error(ERR_NONE, NULL));
}

static t_error	prepare_pipeline(t_ast **ast)
{
	t_ast	*node;
	t_list	*lst_node;

	if (ast == NULL || *ast == NULL || (*ast)->type != NODE_COMMAND)
		return (error(ERR_IMPLEMENTATION, NULL));
	node = create_ast(NODE_PIPELINE);
	if (node == NULL)
		return (error(ERR_ALLOCATION, NULL));
	lst_node = ft_lstnew(*ast);
	if (lst_node == NULL)
		return (error(ERR_ALLOCATION, NULL));
	ft_lstadd_back(&node->pipeline, lst_node);
	*ast = node;
	return (error(ERR_NONE, NULL));
}

static t_error	led_pipe(t_ast **ast, t_list **tk_lst, t_hash_table *env)
{
	t_ast	*right;
	t_token	*token;
	t_error	err;

	if (ast == NULL || tk_lst == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if ((*ast)->type == NODE_COMMAND)
	{
		err = prepare_pipeline(ast);
		if (err.id != ERR_NONE)
			return (err);
	}
	right = NULL;
	token = peek_front(tk_lst, 0);
	if (token == NULL || token->type == TK_PIPE || token->type == TK_AND
		|| token->type == TK_OR || token->type == TK_P_OPEN)
		err = error(ERR_SYNTAX, NULL);
	else
		err = parse_expression(&right, tk_lst, env, get_precedence(TK_PIPE));
	if (err.id != ERR_NONE)
		return (err);
	return (ft_lstadd_back(&(*ast)->pipeline, ft_lstnew(right)),
		error(ERR_NONE, NULL));
}

static t_error	led_logic(t_ast **ast, t_list **tk_lst, t_hash_table *env,
					t_token_type tk_type)
{
	t_ast		*node;
	t_ast		*right;
	t_error		err;
	t_node_type	type;

	if (ast == NULL || tk_lst == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	type = NODE_AND;
	if (tk_type == TK_OR)
		type = NODE_OR;
	node = create_ast(type);
	if (node == NULL)
		return (error(ERR_ALLOCATION, NULL));
	node->left = *ast;
	*ast = node;
	right = NULL;
	err = parse_expression(&right, tk_lst, env, get_precedence(tk_type));
	if (err.id != ERR_NONE)
		return (err);
	if (right == NULL)
		return (error(ERR_SYNTAX, NULL));
	node->right = right;
	return (error(ERR_NONE, NULL));
}

t_error	led(t_ast **ast, t_list **tk_lst, t_hash_table *env, t_token *token)
{
	t_error	err;

	if (ast == NULL || tk_lst == NULL || token == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = error(ERR_SYNTAX, NULL);
	if (token->type == TK_WORD)
	{
		err = led_word(ast, token);
		if (err.id != ERR_NONE)
			return (err);
	}
	else if (token->type == TK_PIPE
		&& ((*ast != NULL && (*ast)->type != NODE_GROUP) || *ast == NULL))
	{
		err = led_pipe(ast, tk_lst, env);
		if (err.id != ERR_NONE)
			return (err);
	}
	else if (token->type == TK_AND || token->type == TK_OR)
	{
		err = led_logic(ast, tk_lst, env, token->type);
		if (err.id != ERR_NONE)
			return (err);
	}
	return (err);
}
