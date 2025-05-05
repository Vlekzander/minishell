/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 16:23:08 by apierret          #+#    #+#             */
/*   Updated: 2025/05/05 13:25:29 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "parser_internal.h"

t_error	process_redirection(t_ast *node, t_token *token, t_list **redirs,
	t_token_type *current_redir)
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
		if (node != NULL && (node->type == NODE_COMMAND || node->type == NODE_SUBSHELL))
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

int	is_end_of_expression(t_list **tk_lst, int precedence)
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
		return (1);
	if (next->type == TK_P_CLOSE && precedence == -1)
		return (1);
	return (0);
}

t_error	handle_expression(t_ast **node, t_list **tk_lst, t_token *token,
	t_list **redirs)
{
	if (*node == NULL)
		return (nud(node, tk_lst, token, redirs));
	return (led(node, tk_lst, token));
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
	while (1)
	{
		if (is_end_of_expression(tk_lst, precedence))
			break ;
		token = pop_front(tk_lst);
		if ((is_redirection(token) && current_redir == TK_NONE)
			|| (token->type == TK_WORD && current_redir != TK_NONE))
			error = process_redirection(node, token, &redirs, &current_redir);
		else if (current_redir == TK_NONE)
			error = handle_expression(&node, tk_lst, token, &redirs);
		else
			error = ERR_SYNTAX;
		if (error != ERR_NONE)
			return (free_ast(node), error);
	}
	if (current_redir != TK_NONE)
		return (free_ast(node), ft_lstclear(&redirs, (void (*)(void *)) free_redir), ERR_SYNTAX);
	if (node == NULL && redirs != NULL)
	{
		node = create_ast(NODE_REDIR);
		if (node == NULL)
			return (ft_lstclear(&redirs, (void (*)(void *)) free_redir), ERR_ALLOCATION);
		node->redirs = redirs;
	}
	return (*ast = node, ERR_NONE);
}

t_error	parse_ast(t_ast **ast, t_list *tokens)
{
	if (ast == NULL || tokens == NULL)
		return (ERR_IMPLEMENTATION);
	return (parse_expression(ast, &tokens, 0));
}
