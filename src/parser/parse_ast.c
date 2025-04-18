/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 16:23:08 by apierret          #+#    #+#             */
/*   Updated: 2025/04/18 22:58:27 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <munit.h>

#include "parser.h"
#include "utils.h"

static int	precedence(t_token_type type)
{
	if (type == TK_WORD)
		return (100);
	if (type == TK_AND || type == TK_OR)
		return (20);
	if (type == TK_PIPE)
		return (30);
	if (type == TK_OUT || type == TK_APPEND || type == TK_IN)
		return (40);
	if (type == TK_HEREDOC)
		return (50);
	return (-1);
}

static t_error	nud(t_ast **ast, t_token *token)
{
	t_ast	*node;
	char	*exec_name;

	if (ast == NULL || token == NULL)
		return (ERR_IMPLEMENTATION);
	if (token->type == TK_WORD)
	{
		node = create_ast(NODE_COMMAND);
		if (node == NULL)
			return (ERR_ALLOCATION);
		node->command->path = ft_strdup(token->value);
		if (node->command->path == NULL)
			return (free_ast(node), ERR_ALLOCATION);
		exec_name = ft_strrchr(token->value, '/');
		if (exec_name != NULL)
			exec_name++;
		else
			exec_name = token->value;
		str_array_push(&node->command->args, exec_name);
	}
	else
		return (ERR_ALLOCATION);
	return (*ast = node, ERR_NONE);
}

static t_error	led(t_ast **ast, t_token *token, t_ast *left)
{
	t_error	ret;

	if (ast == NULL || token == NULL || left == NULL)
		return (ERR_IMPLEMENTATION);
	if (left->type != NODE_COMMAND)
		return (ERR_SYNTAX);
	ret = str_array_push(&left->command->args, token->value);
	if (ret != ERR_NONE)
		return (ret);
	*ast = left;
	return (ERR_NONE);
}

static t_error	parse_expr(t_ast **ast, t_list *tokens, int min_prec)
{
	t_token	*token;
	t_ast	*left;
	t_error	error;

	if (ast == NULL || tokens == NULL)
		return (ERR_IMPLEMENTATION);
	token = (t_token *) tokens->content;
	tokens = tokens->next;
	error = nud(&left, token);
	if (error != ERR_NONE)
		return (error);
	while (tokens != NULL)
	{
		token = (t_token *) tokens->content;
		if (precedence(token->type) < min_prec)
			break ;
		tokens = tokens->next;
		error = led(&left, token, left);
		if (error != ERR_NONE)
			return (error);
	}
	*ast = left;
	return (ERR_NONE);
}

t_error	parse_ast(t_ast **ast, t_list *tokens)
{
	if (ast == NULL || tokens == NULL)
		return (ERR_IMPLEMENTATION);
	return (parse_expr(ast, tokens, 0));
}
