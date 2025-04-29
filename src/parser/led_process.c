/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   led_process.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:41:28 by apierret          #+#    #+#             */
/*   Updated: 2025/04/29 15:07:43 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "utils.h"

t_error	parse_expression(t_ast **ast, t_list **tk_list, int precedence);
int		get_precedence(t_token_type type);

t_error led_word(t_ast **ast, t_token *token)
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

t_error	led_pipe(t_ast **ast, t_list **tk_list)
{
	t_ast	*old_node;
	t_ast	*node;
	t_ast	*right;
	t_error	error;

	if (ast == NULL || tk_list == NULL)
		return (ERR_IMPLEMENTATION);
	if ((*ast)->type == NODE_COMMAND)
	{
		old_node = *ast;
		node = create_ast(NODE_PIPELINE);
		if (node == NULL)
			return (ERR_ALLOCATION);
		ft_lstadd_back(&node->pipeline, ft_lstnew(old_node));
		*ast = node;
	}
	right = NULL;
	error = parse_expression(&right, tk_list, get_precedence(TK_PIPE));
	if (error != ERR_NONE)
		return (error);
	ft_lstadd_back(&(*ast)->pipeline, ft_lstnew(right));
	return (ERR_NONE);
}
