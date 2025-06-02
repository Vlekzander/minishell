/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_dup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:31:53 by apierret          #+#    #+#             */
/*   Updated: 2025/06/02 14:48:01 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_utils.h"

t_list	*lst_dup(t_list *src, void *(*dup)(void *), void (*del)(void *))
{
	t_list	*lst;
	t_list	*node;
	void	*ptr;

	lst = NULL;
	while (src != NULL)
	{
		ptr = dup(src->content);
		if (ptr == NULL)
			return (ft_lstclear(&lst, del), NULL);
		node = ft_lstnew(ptr);
		if (node == NULL)
			return (del(ptr), ft_lstclear(&lst, del), NULL);
		ft_lstadd_back(&lst, node);
		src = src->next;
	}
	return (lst);
}
