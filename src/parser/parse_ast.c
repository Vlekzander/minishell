/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 16:23:08 by apierret          #+#    #+#             */
/*   Updated: 2025/07/08 12:39:00 by apierret         ###   ########.fr       */
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
		return (error(ERR_IMPLEMENTATION, NULL));
	if (*current_redir == TK_NONE)
		return (*current_redir = token->type, error(ERR_NONE, NULL));
	if (*node == NULL)
	{
		*node = create_ast(NODE_REDIR);
		if (*node == NULL)
			return (error(ERR_ALLOCATION, NULL));
	}
	list = &(*node)->redirs;
	redir_type = get_redir_type(*current_redir);
	redir = create_redir(redir_type, token->value, *current_redir == TK_APPEND);
	if (redir == NULL)
		return (error(ERR_ALLOCATION, NULL));
	element = ft_lstnew(redir);
	if (element == NULL)
		return (free_redir(redir), error(ERR_ALLOCATION, NULL));
	ft_lstadd_back(list, element);
	return (*current_redir = TK_NONE, error(ERR_NONE, NULL));
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

static t_error	finalize_expression(t_ast **ast, t_hash_table *env, t_ast *node,
					t_token_type current_redir)
{
	t_error	err;

	if (ast == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = error(ERR_NONE, NULL);
	if (node != NULL
		&& (node->type == NODE_COMMAND || node->type == NODE_REDIR))
		err = prompt_redirs(node->redirs, env);
	if (node != NULL && node->type == NODE_PIPELINE)
		err = prompt_redirs(((t_ast *) node->pipeline->content)->redirs, env);
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

t_error	parse_expression(t_ast **ast, t_list **tk_lst, t_hash_table *env,
			int prec)
{
	t_ast			*node;
	t_token			*token;
	t_error			err;
	t_token_type	current_redir;

	if (ast == NULL || tk_lst == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	node = NULL;
	current_redir = TK_NONE;
	while (in_expression(&token, tk_lst, prec))
	{
		err = error(ERR_SYNTAX, NULL);
		if (((node != NULL && node->type != NODE_GROUP) || node == NULL)
			&& ((is_redirection(token) && current_redir == TK_NONE)
				|| (token->type == TK_WORD && current_redir != TK_NONE)))
			err = process_redirection(&node, token, &current_redir);
		else if (current_redir == TK_NONE
			&& (node == NULL || node->type == NODE_REDIR))
			err = nud(&node, tk_lst, env, token);
		else if (current_redir == TK_NONE)
			err = led(&node, tk_lst, env, token);
		if (err.id != ERR_NONE)
			return (free_ast(node), err);
	}
	return (finalize_expression(ast, env, node, current_redir));
}

t_error	parse_ast(t_ast **ast, t_list *tokens, t_hash_table *env)
{
	if (ast == NULL || tokens == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	return (parse_expression(ast, &tokens, env, 0));
}
