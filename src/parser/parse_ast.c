/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 16:23:08 by apierret          #+#    #+#             */
/*   Updated: 2025/04/23 15:22:34 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <munit.h>

#include "parser.h"
#include "utils.h"

static t_token	*advance(t_list **tk_list)
{
	t_token	*token;

	if (tk_list == NULL || *tk_list == NULL || (*tk_list)->content == NULL)
		return (NULL);
	token = (t_token *)(*tk_list)->content;
	*tk_list = (*tk_list)->next;
	return (token);
}

static int	precedence(t_token_type type)
{
	if (type == TK_WORD)
		return (100);
	if (type == TK_AND || type == TK_OR)
		return (1);
	if (type == TK_PIPE)
		return (2);
	return (-1);
}

static t_error	nud(t_ast **ast, t_token *token, t_list **redirs)
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
		if (*redirs != NULL)
			node->command->redirs = *redirs;
		else
			*redirs = node->command->redirs;
	}
	else
		return (ERR_SYNTAX);
	return (*ast = node, ERR_NONE);
}

static t_error	led(t_ast **ast, t_token *token)
{
	t_error	ret;

	if (ast == NULL || token == NULL)
		return (ERR_IMPLEMENTATION);
	if ((*ast)->type == NODE_COMMAND)
	{
		ret = str_array_push(&(*ast)->command->args, token->value);
		if (ret != ERR_NONE)
			return (ret);
	}
	else
		return (ERR_SYNTAX);
	return (ERR_NONE);
}

static t_error	parse_expression(t_ast **ast, t_list *tk_list, int brp)
{
	t_token			*token;
	t_ast			*left;
	t_error			error;
	t_list			*redirs;
	t_token_type	current_redir;

	if (ast == NULL || tk_list == NULL)
		return (ERR_IMPLEMENTATION);
	left = NULL;
	redirs = NULL;
	current_redir = TK_NONE;
	token = advance(&tk_list);
	while (token != NULL)
	{
		if (token->type == TK_IN || token->type == TK_OUT || token->type == TK_APPEND || token->type == TK_HEREDOC || (token->type == TK_WORD && current_redir != TK_NONE))
		{
			if (current_redir == TK_NONE)
				current_redir = token->type;
			else
			{
				if (left != NULL && left->type == NODE_COMMAND)
					ft_lstadd_back(&left->command->redirs, ft_lstnew(create_redir(current_redir, token->value)));
				else
					ft_lstadd_back(&redirs, ft_lstnew(create_redir(current_redir, token->value)));
				current_redir = TK_NONE;
			}

			token = advance(&tk_list);
			continue ;
		}
		if (left == NULL)
		{
			error = nud(&left, token, &redirs);
			if (error != ERR_NONE)
				return (free_ast(left), error);
		}
		else
		{
			if (precedence(token->type) <= brp)
			{
				if (left == NULL && redirs != NULL)
				{
					left = create_ast(NODE_COMMAND);
					if (!left)
						return (ERR_ALLOCATION);
					left->command->redirs = redirs;
				}
				break ;
			}
			error = led(&left, token);
			if (error != ERR_NONE)
				return (free_ast(left), error);
		}
		token = advance(&tk_list);
	}
	if (left == NULL && redirs != NULL)
	{
		left = create_ast(NODE_COMMAND);
		if (!left)
			return (ERR_ALLOCATION);
		left->command->redirs = redirs;
	}
	*ast = left;
	return (ERR_NONE);
}

t_error	parse_ast(t_ast **ast, t_list *tokens)
{
	if (ast == NULL || tokens == NULL)
		return (ERR_IMPLEMENTATION);
	return (parse_expression(ast, tokens, 0));
}
