/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 16:23:08 by apierret          #+#    #+#             */
/*   Updated: 2025/05/05 15:56:17 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "parser_internal.h"

static t_error	process_redirection(t_ast *node, t_token *token,
	t_list **redirs, t_token_type *current_redir)
{
	t_list	**list;
	t_list	*element;
	t_redir	*redir;

	if (token == NULL || redirs == NULL || current_redir == NULL)
		return (ERR_IMPLEMENTATION);
	if (*current_redir == TK_NONE)
		*current_redir = token->type;
	else
	{
		list = redirs;
		if (node != NULL && (node->type == NODE_COMMAND
				|| node->type == NODE_SUBSHELL))
			list = &node->redirs;
		redir = create_redir(*current_redir, token->value);
		if (redir == NULL)
			return (ERR_ALLOCATION);
		element = ft_lstnew(redir);
		if (element == NULL)
			return (ERR_ALLOCATION);
		ft_lstadd_back(list, element);
		*current_redir = TK_NONE;
	}
	return (ERR_NONE);
}

static int	in_expression(t_token **token, t_list **tk_lst, int precedence)
{
	t_token	*next;

	next = peek_front(*tk_lst);
	if (next == NULL
		|| (next->type != TK_IN
			&& next->type != TK_OUT
			&& next->type != TK_APPEND
			&& next->type != TK_HEREDOC
			&& next->type != TK_P_OPEN
			&& next->type != TK_P_CLOSE
			&& get_precedence(next->type) <= precedence)
		|| (next->type == TK_P_CLOSE && precedence > 0))
		return (0);
	if (next->type == TK_P_CLOSE && precedence == -1)
		return (0);
	return (*token = pop_front(tk_lst), 1);
}

static t_error	finalize_expression(t_ast **ast, t_ast *node,
	t_token_type current_redir, t_list *redirs)
{
	if (ast == NULL)
		return (ERR_IMPLEMENTATION);
	if (current_redir != TK_NONE)
		return (free_ast(node), clear_redirs(&redirs), ERR_SYNTAX);
	if (node == NULL && redirs != NULL)
	{
		node = create_ast(NODE_REDIR);
		if (node == NULL)
			return (clear_redirs(&redirs), ERR_ALLOCATION);
		node->redirs = redirs;
	}
	return (*ast = node, ERR_NONE);
}

t_error	parse_expression(t_ast **ast, t_list **tk_lst, int precedence)
{
	t_ast			*node;
	t_token			*token;
	t_list			*redirs;
	t_error			error;
	t_token_type	current_redir;

	if (ast == NULL || tk_lst == NULL)
		return (ERR_IMPLEMENTATION);
	node = NULL;
	redirs = NULL;
	current_redir = TK_NONE;
	while (in_expression(&token, tk_lst, precedence))
	{
		error = ERR_SYNTAX;
		if ((is_redirection(token) && current_redir == TK_NONE)
			|| (token->type == TK_WORD && current_redir != TK_NONE))
			error = process_redirection(node, token, &redirs, &current_redir);
		else if (current_redir == TK_NONE && node == NULL)
			error = nud(&node, tk_lst, token, &redirs);
		else if (current_redir == TK_NONE)
			error = led(&node, tk_lst, token);
		if (error != ERR_NONE)
			return (free_ast(node), clear_redirs(&redirs), error);
	}
	return (finalize_expression(ast, node, current_redir, redirs));
}

t_error	parse_ast(t_ast **ast, t_list *tokens)
{
	if (ast == NULL || tokens == NULL)
		return (ERR_IMPLEMENTATION);
	return (parse_expression(ast, &tokens, 0));
}
