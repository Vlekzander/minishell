/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   htable_foreach.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 11:42:44 by apierret          #+#    #+#             */
/*   Updated: 2025/06/19 12:01:19 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hash.h"

int	htable_foreach(t_hash_table *htable,
			int (*f)(char *key, void *content, void *data), void *data)
{
	size_t		i;
	t_list		*lst;
	t_hash_node	*node;
	int			ret;

	if (htable == NULL || f == NULL)
		return (-2);
	i = 0;
	while (i < htable->buckets_count)
	{
		lst = htable->buckets[i];
		while (lst != NULL)
		{
			node = lst->content;
			if (node != NULL)
			{
				ret = f(node->key, node->content, data);
				if (ret != 0)
					return (ret);
			}
			lst = lst->next;
		}
		i++;
	}
	return (0);
}
