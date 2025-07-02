/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_remove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 21:42:37 by apierret          #+#    #+#             */
/*   Updated: 2025/07/01 21:47:55 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

static void	remove_node(t_list **lst, t_list **prev, t_list **node,
							void (*del)(void *))
{
	t_list	*temp;

	temp = *node;
	if (*prev == NULL)
	{
		*lst = (*node)->next;
		*node = *lst;
	}
	else
	{
		(*prev)->next = (*node)->next;
		*node = (*prev)->next;
	}
	ft_lstdelone(temp, del);
}

void	lst_remove(t_list **lst, int (*f)(void *), void (*del)(void *))
{
	t_list	*prev;
	t_list	*node;

	if (lst == NULL)
		return ;
	prev = NULL;
	node = *lst;
	while (node != NULL)
	{
		if (f(node->content))
		{
			remove_node(lst, &prev, &node, del);
			continue ;
		}
		prev = node;
		node = node->next;
	}
}
