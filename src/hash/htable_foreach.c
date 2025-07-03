/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   htable_foreach.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 11:42:44 by apierret          #+#    #+#             */
/*   Updated: 2025/07/03 14:17:37 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hash.h"

int	htable_foreach(t_hash_table *htable,
			int (*f)(char *key, void *content, void *data), void *data)
{
	size_t		i;
	t_list		*lst;
	t_hash_node	*node;

	if (htable == NULL || f == NULL)
		return (0);
	i = 0;
	while (i < htable->buckets_count)
	{
		lst = htable->buckets[i];
		while (lst != NULL)
		{
			node = lst->content;
			if (node != NULL)
			{
				if (!f(node->key, node->content, data))
					return (0);
			}
			lst = lst->next;
		}
		i++;
	}
	return (1);
}
