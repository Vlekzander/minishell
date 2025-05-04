/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nud_process.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 12:24:12 by apierret          #+#    #+#             */
/*   Updated: 2025/05/04 15:10:35 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"
#include "error.h"
#include "utils.h"

extern t_error	parse_expression(t_ast **ast, t_list **tk_lst, int precedence);

t_error	nud_word(t_ast **ast, t_token *token, t_list **redirs)
{
	t_ast	*node;
	char	*basename;

	if (ast == NULL || token == NULL)
		return (ERR_IMPLEMENTATION);
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
	return (*ast = node, ERR_NONE);
}

t_error	nud_subshell(t_ast **ast, t_list **tk_lst, t_list **redirs)
{
	t_ast	*node;
	t_error	error;

	if (*redirs != NULL)
	{
		ft_lstclear(redirs, (void (*)(void *)) free_redir);
		return (ERR_SYNTAX);
	}
	node = create_ast(NODE_SUBSHELL);
	if (node == NULL)
		return (ERR_ALLOCATION);
	error = parse_expression(&node->child, tk_lst, 0);
	if (error != ERR_NONE)
		return (free_ast(node), error);
	return (*ast = node, ERR_NONE);
}

t_error	nud(t_ast **ast, t_list **tk_lst, t_token *token, t_list **rds)
{
	t_error	error;

	if (ast == NULL || token == NULL)
		return (ERR_IMPLEMENTATION);
	if (token->type == TK_WORD)
	{
		error = nud_word(ast, token, rds);
		if (error != ERR_NONE)
			return (error);
	}
	else if (token->type == TK_P_OPEN)
	{
		error = nud_subshell(ast, tk_lst, rds);
		if (error != ERR_NONE)
			return (error);
	}
	else
		return (ERR_SYNTAX);
	return (ERR_NONE);
}
