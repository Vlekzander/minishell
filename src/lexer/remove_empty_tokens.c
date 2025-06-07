/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_empty_tokens.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 16:17:14 by apierret          #+#    #+#             */
/*   Updated: 2025/06/07 18:23:12 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	remove_node(t_list *node, t_list *prev, t_list **tk_list)
{
	if (node == NULL || tk_list == NULL)
		return ;
	if (prev != NULL)
		prev->next = node->next;
	else
		*tk_list = node->next;
}

t_error	remove_empty_tokens(t_list **tk_list)
{
	t_list	*node;
	t_list	*prev;
	t_list	*del;
	t_token	*token;

	if (tk_list == NULL)
		return (ERR_IMPLEMENTATION);
	prev = NULL;
	node = *tk_list;
	while (node != NULL)
	{
		del = NULL;
		token = node->content;
		if (ft_strlen(token->value) == 0)
		{
			remove_node(node, prev, tk_list);
			del = node;
		}
		else
			prev = node;
		node = node->next;
		ft_lstdelone(del, (void *) free_token);
	}
	return (ERR_NONE);
}
