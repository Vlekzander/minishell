/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 16:23:08 by apierret          #+#    #+#             */
/*   Updated: 2025/06/16 15:26:12 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "redirs.h"

static t_error	process_redirection(t_ast **node, t_token *token,
	t_token_type *current_redir)
{
	t_list			**list;
	t_list			*element;
	t_redir			*redir;
	t_redir_type	redir_type;

	if (node == NULL || token == NULL || current_redir == NULL)
		return (ERR_IMPLEMENTATION);
	if (*current_redir == TK_NONE)
		return (*current_redir = token->type, ERR_NONE);
	if (*node == NULL)
	{
		*node = create_ast(NODE_REDIR);
		if (*node == NULL)
			return (ERR_ALLOCATION);
	}
	list = &(*node)->redirs;
	redir_type = get_redir_type(*current_redir);
	redir = create_redir(redir_type, token->value, *current_redir == TK_APPEND);
	if (redir == NULL)
		return (ERR_ALLOCATION);
	element = ft_lstnew(redir);
	if (element == NULL)
		return (free_redir(redir), ERR_ALLOCATION);
	ft_lstadd_back(list, element);
	return (*current_redir = TK_NONE, ERR_NONE);
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

static t_error	finalize_expression(t_ast **ast, char **env, t_ast *node,
	t_token_type current_redir)
{
	t_error	error;

	if (ast == NULL)
		return (ERR_IMPLEMENTATION);
	error = ERR_NONE;
	if (node != NULL)
		error = prompt_redirs(node->redirs, env);
	if (error != ERR_NONE)
		return (free_ast(node), ERR_SYNTAX);
	if (current_redir != TK_NONE)
		return (free_ast(node), ERR_SYNTAX);
	return (*ast = node, ERR_NONE);
}

t_error	parse_expression(t_ast **ast, t_list **tk_lst, char **env, int prec)
{
	t_ast			*node;
	t_token			*token;
	t_error			error;
	t_token_type	current_redir;

	if (ast == NULL || tk_lst == NULL)
		return (ERR_IMPLEMENTATION);
	node = NULL;
	current_redir = TK_NONE;
	while (in_expression(&token, tk_lst, prec))
	{
		error = ERR_SYNTAX;
		if (((node != NULL && node->type != NODE_GROUP) || node == NULL)
			&& ((is_redirection(token) && current_redir == TK_NONE)
				|| (token->type == TK_WORD && current_redir != TK_NONE)))
			error = process_redirection(&node, token, &current_redir);
		else if (current_redir == TK_NONE
			&& (node == NULL || node->type == NODE_REDIR))
			error = nud(&node, tk_lst, env, token);
		else if (current_redir == TK_NONE)
			error = led(&node, tk_lst, env, token);
		if (error != ERR_NONE)
			return (free_ast(node), error);
	}
	return (finalize_expression(ast, env, node, current_redir));
}

t_error	parse_ast(t_ast **ast, t_list *tokens, char **env)
{
	if (ast == NULL || tokens == NULL)
		return (ERR_IMPLEMENTATION);
	return (parse_expression(ast, &tokens, env, 0));
}
