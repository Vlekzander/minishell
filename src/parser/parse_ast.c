/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 16:23:08 by apierret          #+#    #+#             */
/*   Updated: 2025/07/19 12:26:44 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "redirs.h"

static t_error	process_redirection(t_ast **node, t_token *token, t_list **hds,
	t_token_type *current_redir)
{
	t_redir			*redir;
	t_error			err;

	if (node == NULL || token == NULL || hds == NULL || current_redir == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (*current_redir == TK_NONE)
		return (*current_redir = token->type, error(ERR_NONE, NULL));
	if (*node == NULL)
	{
		*node = create_ast(NODE_REDIR);
		if (*node == NULL)
			return (error(ERR_ALLOCATION, NULL));
	}
	redir = create_redir(get_redir_type(*current_redir), token->value,
			*current_redir == TK_APPEND);
	if (redir == NULL)
		return (error(ERR_ALLOCATION, NULL));
	err = redir_to_list(&(*node)->redirs, redir);
	if (err.id != ERR_NONE)
		return (free_redir(redir), err);
	if (redir->type == REDIR_HEREDOC)
		err = redir_to_list(hds, redir);
	return (*current_redir = TK_NONE, err);
}

static int	in_expression(t_token **token, t_list **tk_lst, int prec)
{
	t_token	*next;

	next = peek_front(tk_lst, 0);
	if (next == NULL
		|| (next->type != TK_IN
			&& next->type != TK_OUT
			&& next->type != TK_APPEND
			&& next->type != TK_HEREDOC
			&& next->type != TK_P_OPEN
			&& next->type != TK_P_CLOSE
			&& get_precedence(next->type) <= prec)
		|| (next->type == TK_P_CLOSE && prec > 0))
		return (0);
	if (next->type == TK_P_CLOSE && prec == -1)
		return (0);
	return (*token = peek_front(tk_lst, 1), 1);
}

static t_error	finalize_expression(t_ast **ast, t_ast *node,
	t_token_type current_redir)
{
	t_error	err;

	if (ast == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = error(ERR_NONE, NULL);
	if (err.id != ERR_NONE)
	{
		if (err.id == ERR_FILE_NOT_FOUND)
			return (free_ast(node), error(ERR_HEREDOC_FILE, NULL));
		return (free_ast(node), error(ERR_SYNTAX, NULL));
	}
	if (current_redir != TK_NONE)
		return (free_ast(node), error(ERR_SYNTAX, NULL));
	return (*ast = node, error(ERR_NONE, NULL));
}

t_error	parse_expression(t_ast **ast, t_list **tk_lst, t_list **hds, int prec)
{
	t_ast			*node;
	t_token			*token;
	t_error			err;
	t_token_type	redir;

	if (ast == NULL || tk_lst == NULL || hds == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	node = NULL;
	redir = TK_NONE;
	while (in_expression(&token, tk_lst, prec))
	{
		err = error(ERR_SYNTAX, NULL);
		if (redir_cond(node, token, redir))
			err = process_redirection(&node, token, hds, &redir);
		else if (redir == TK_NONE && (node == NULL || node->type == NODE_REDIR))
			err = nud(&node, tk_lst, token, hds);
		else if (redir == TK_NONE)
			err = led(&node, tk_lst, token, hds);
		if (redir == TK_NONE && node != NULL
			&& node->type == NODE_REDIR && token->type == TK_PIPE)
			err = led(&node, tk_lst, token, hds);
		if (err.id != ERR_NONE)
			return (free_ast(node), err);
	}
	return (finalize_expression(ast, node, redir));
}

t_error	parse_ast(t_ast **ast, t_list *tokens, t_hash_table *env)
{
	t_error	err;
	t_list	*hds;

	if (ast == NULL || tokens == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	hds = NULL;
	err = parse_expression(ast, &tokens, &hds, 0);
	if (err.id != ERR_NONE)
		return (ft_lstclear(&hds, NULL), err);
	err = prompt_redirs(hds, env);
	ft_lstclear(&hds, NULL);
	return (err);
}
