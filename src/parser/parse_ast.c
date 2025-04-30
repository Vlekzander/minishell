/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 16:23:08 by apierret          #+#    #+#             */
/*   Updated: 2025/04/30 14:37:32 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "utils.h"

t_error	led_word(t_ast **ast, t_token *token);
t_error	led_pipe(t_ast **ast, t_list **tk_list);
t_error	led_logic(t_ast **ast, t_list **tk_list, t_token_type tk_type);

static t_token	*pop_front(t_list **tk_list)
{
	t_token	*token;

	if (tk_list == NULL || *tk_list == NULL || (*tk_list)->content == NULL)
		return (NULL);
	token = (t_token *)(*tk_list)->content;
	*tk_list = (*tk_list)->next;
	return (token);
}

static t_token	*peek_front(t_list *tk_list)
{
	t_token	*token;

	if (tk_list == NULL || tk_list->content == NULL)
		return (NULL);
	token = (t_token *)tk_list->content;
	return (token);
}

int	get_precedence(t_token_type type)
{
	if (type == TK_WORD)
		return (100);
	if (type == TK_AND || type == TK_OR)
		return (1);
	if (type == TK_PIPE)
		return (2);
	return (0);
}

static t_error nud(t_ast **ast, t_token *token, t_list	**redirs)
{
	t_ast	*node;
	char	*basename;

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
		basename = ft_strrchr(token->value, '/');
		if (basename != NULL)
			basename++;
		else
			basename = token->value;
		str_array_push(&node->command->args, basename);
		if (*redirs != NULL)
			node->command->redirs = *redirs;
		else
			*redirs = node->command->redirs;
	}
	else
		return (ERR_SYNTAX);
	*ast = node;
	return (ERR_NONE);
}

static t_error led(t_ast **ast, t_list **tk_list, t_token *token)
{
	t_error	error;

	if (ast == NULL || tk_list == NULL || token == NULL)
		return (ERR_IMPLEMENTATION);
	if (token->type == TK_WORD)
	{
		error = led_word(ast, token);
		if (error != ERR_NONE)
			return (error);
	}
	else if (token->type == TK_PIPE)
	{
		error = led_pipe(ast, tk_list);
		if (error != ERR_NONE)
			return (error);
	}
	else if (token->type == TK_AND || token->type == TK_OR)
	{
		error = led_logic(ast, tk_list, token->type);
		if (error != ERR_NONE)
			return (error);
	}
	else
		return (ERR_SYNTAX);
	return (ERR_NONE);
}

t_error	parse_expression(t_ast **ast, t_list **tk_list, int precedence)
{
	t_ast			*node;
	t_token			*token;
	t_token			*next;
	t_error			error;
	t_list			*redirs;
	t_token_type	current_redir;

	if (ast == NULL || tk_list == NULL || *tk_list == NULL)
		return (ERR_IMPLEMENTATION);
	node = NULL;
	redirs = NULL;
	current_redir = TK_NONE;
	while (1)
	{
		next = peek_front(*tk_list);
		if (next == NULL)
			break;
		if (next->type != TK_IN && next->type != TK_OUT && next->type != TK_APPEND && next->type != TK_HEREDOC && get_precedence(next->type) <= precedence)
			break;
		token = pop_front(tk_list);
		if (token->type == TK_IN || token->type == TK_OUT || token->type == TK_APPEND || token->type == TK_HEREDOC || (token->type == TK_WORD && current_redir != TK_NONE))
		{
			if (current_redir == TK_NONE)
				current_redir = token->type;
			else
			{
				if (node != NULL && node->type == NODE_COMMAND)
					ft_lstadd_back(&node->command->redirs, ft_lstnew(create_redir(current_redir, token->value)));
				else
					ft_lstadd_back(&redirs, ft_lstnew(create_redir(current_redir, token->value)));
				current_redir = TK_NONE;
			}
			continue ;
		}
		if (node == NULL)
		{
			error = nud(&node, token, &redirs);
			if (error != ERR_NONE)
				return (error);
		}
		else
		{
			error = led(&node, tk_list, token);
			if (error != ERR_NONE)
				return (error);
		}
	}
	return (*ast = node, ERR_NONE);
}

t_error	parse_ast(t_ast **ast, t_list *tokens)
{
	if (ast == NULL || tokens == NULL)
		return (ERR_IMPLEMENTATION);
	return (parse_expression(ast, &tokens, 0));
}
