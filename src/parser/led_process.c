/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   led_process.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:41:28 by apierret          #+#    #+#             */
/*   Updated: 2025/05/04 15:57:33 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"
#include "error.h"
#include "parser_internal.h"
#include "utils.h"

t_error	led_word(t_ast **ast, t_token *token)
{
	t_error	error;
	t_ast	*node;

	if (ast == NULL || token == NULL)
		return (ERR_IMPLEMENTATION);
	if ((*ast)->type == NODE_COMMAND)
		node = (*ast);
	else if ((*ast)->type == NODE_PIPELINE)
		node = (t_ast *) ft_lstlast((*ast)->pipeline)->content;
	else
		return (ERR_SYNTAX);
	if (node == NULL || node->command == NULL)
		return (ERR_IMPLEMENTATION);
	error = str_array_push(&node->command->args, token->value);
	if (error != ERR_NONE)
		return (error);
	return (ERR_NONE);
}

t_error	led_pipe(t_ast **ast, t_list **tk_lst)
{
	t_ast	*old_node;
	t_ast	*node;
	t_ast	*right;
	t_error	error;

	if (ast == NULL || tk_lst == NULL)
		return (ERR_IMPLEMENTATION);
	if ((*ast)->type == NODE_COMMAND || (*ast)->type == NODE_SUBSHELL)
	{
		old_node = *ast;
		node = create_ast(NODE_PIPELINE);
		if (node == NULL)
			return (ERR_ALLOCATION);
		ft_lstadd_back(&node->pipeline, ft_lstnew(old_node));
		*ast = node;
	}
	right = NULL;
	error = parse_expression(&right, tk_lst, get_precedence(TK_PIPE));
	if (error != ERR_NONE)
		return (error);
	ft_lstadd_back(&(*ast)->pipeline, ft_lstnew(right));
	return (ERR_NONE);
}

t_error	led_logic(t_ast **ast, t_list **tk_lst, t_token_type tk_type)
{
	t_ast		*old_node;
	t_ast		*node;
	t_ast		*right;
	t_error		error;
	t_node_type	type;

	if (ast == NULL || tk_lst == NULL)
		return (ERR_IMPLEMENTATION);
	old_node = *ast;
	type = NODE_AND;
	if (tk_type == TK_OR)
		type = NODE_OR;
	node = create_ast(type);
	if (node == NULL)
		return (ERR_ALLOCATION);
	node->left = old_node;
	*ast = node;
	right = NULL;
	error = parse_expression(&right, tk_lst, get_precedence(tk_type));
	if (error != ERR_NONE)
		return (error);
	node->right = right;
	return (ERR_NONE);
}

t_error	led(t_ast **ast, t_list **tk_lst, t_token *token)
{
	t_error	error;

	if (ast == NULL || tk_lst == NULL || token == NULL)
		return (ERR_IMPLEMENTATION);
	if (token->type == TK_WORD)
	{
		error = led_word(ast, token);
		if (error != ERR_NONE)
			return (error);
	}
	else if (token->type == TK_PIPE)
	{
		error = led_pipe(ast, tk_lst);
		if (error != ERR_NONE)
			return (error);
	}
	else if (token->type == TK_AND || token->type == TK_OR)
	{
		error = led_logic(ast, tk_lst, token->type);
		if (error != ERR_NONE)
			return (error);
	}
	else
		return (ERR_SYNTAX);
	return (ERR_NONE);
}
