/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 10:27:21 by apierret          #+#    #+#             */
/*   Updated: 2024/10/04 21:26:17 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*ret_lst;
	t_list	*elem;
	void	*content;

	if (lst == NULL)
		return (NULL);
	ret_lst = NULL;
	elem = NULL;
	while (lst != NULL)
	{
		content = f(lst->content);
		elem = ft_lstnew(content);
		if (elem == NULL)
		{
			del(content);
			return (ft_lstclear(&ret_lst, del), NULL);
		}
		ft_lstadd_back(&ret_lst, elem);
		lst = lst->next;
	}
	return (ret_lst);
}
