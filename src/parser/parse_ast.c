/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 16:23:08 by apierret          #+#    #+#             */
/*   Updated: 2025/04/03 16:02:25 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <munit.h>

#include "parser.h"
#include "utils.h"

static t_error	parse_command(t_ast **ast, t_list *tokens)
{
	t_ast		*node;
	t_token		*token;

	if (ast == NULL || tokens == NULL)
		return (ERR_IMPLEMENTATION);
	node = ft_calloc(1, sizeof(t_ast));
	if (node == NULL)
		return (ERR_ALLOCATION);
	node->type = NODE_COMMAND;
	node->command = ft_calloc(1, sizeof(t_command));
	if (node->command == NULL)
		return (free_ast(node), ERR_ALLOCATION);
	while (tokens != NULL)
	{
		token = tokens->content;
		if (token->type == TK_OUT || token->type == TK_APPEND)
		{
			if (tokens->next == NULL)
				return (free_command(node->command), ERR_SYNTAX);
			tokens = tokens->next;
			token = tokens->content;
			node->command->redir.out = ft_strdup(token->value);
			node->command->redir.append = token->type == TK_APPEND;
		}
		else if (token->type == TK_IN)
		{
			if (tokens->next == NULL)
				return (free_command(node->command), ERR_SYNTAX);
			tokens = tokens->next;
			token = tokens->content;
			node->command->redir.in = ft_strdup(token->value);
		}
		else if (token->type == TK_HEREDOC)
		{
			if (tokens->next == NULL)
				return (free_command(node->command), ERR_SYNTAX);
			tokens = tokens->next;
			token = tokens->content;
			node->command->redir.heredoc = ft_strdup(token->value);
		}
		else if (token->type == TK_WORD)
		{
			if (node->command->path == NULL)
				node->command->path = ft_strdup(token->value);
			node->command->args = str_array_push(node->command->args, token->value);
		}
		tokens = tokens->next;
	}
	*ast = node;
	return (ERR_NONE);
}

t_error	parse_ast(t_ast **ast, t_list *tokens)
{
	if (ast == NULL || tokens == NULL)
		return (ERR_IMPLEMENTATION);
	return (parse_command(ast, tokens));
}
