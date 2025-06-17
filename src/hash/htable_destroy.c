/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   htable_destroy.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 23:24:46 by apierret          #+#    #+#             */
/*   Updated: 2025/06/17 13:54:03 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "hash.h"

void	htable_destroy(t_hash_table *htable)
{
	size_t		i;
	t_list		*lst;
	t_hash_node	*node;

	if (htable == NULL)
		return ;
	i = 0;
	while (i < htable->buckets_count)
	{
		lst = htable->buckets[i];
		while (lst != NULL)
		{
			node = lst->content;
			free_hnode(node, htable->del);
			lst = lst->next;
		}
		ft_lstclear(&htable->buckets[i], NULL);
		i++;
	}
	free(htable->buckets);
	free(htable);
}
