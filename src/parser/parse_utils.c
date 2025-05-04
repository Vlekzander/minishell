/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 15:08:08 by apierret          #+#    #+#             */
/*   Updated: 2025/05/04 15:09:41 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"

t_token	*pop_front(t_list **tk_lst)
{
	t_token	*token;

	if (tk_lst == NULL || *tk_lst == NULL || (*tk_lst)->content == NULL)
		return (NULL);
	token = (t_token *)(*tk_lst)->content;
	*tk_lst = (*tk_lst)->next;
	return (token);
}

t_token	*peek_front(t_list *tk_lst)
{
	t_token	*token;

	if (tk_lst == NULL || tk_lst->content == NULL)
		return (NULL);
	token = (t_token *)tk_lst->content;
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

int	is_redirection(t_token *token)
{
	return (token->type == TK_IN || token->type == TK_OUT
		|| token->type == TK_APPEND || token->type == TK_HEREDOC);
}
