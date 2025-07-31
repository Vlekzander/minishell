/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_sublist.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 12:12:19 by apierret          #+#    #+#             */
/*   Updated: 2025/07/31 12:13:29 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "utils.h"

void	insert_sublist(t_list *list, t_list *sublist)
{
	void	*temp;

	if (list == NULL || sublist == NULL)
		return ;
	temp = list->content;
	ft_lstadd_back(&sublist, list->next);
	list->next = sublist->next;
	list->content = sublist->content;
	free(sublist);
	free(temp);
}
