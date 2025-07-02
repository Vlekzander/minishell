/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_sublist.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:44:04 by apierret          #+#    #+#             */
/*   Updated: 2025/07/01 17:57:17 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

t_error	insert_sublist(t_list **list, t_list *sublist)
{
	t_list	*next;

	if (list == NULL || sublist == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	next = (*list)->next;
	(*list)->next = sublist;
	ft_lstadd_back(&sublist, next);
	return (error(ERR_NONE, NULL));
}
