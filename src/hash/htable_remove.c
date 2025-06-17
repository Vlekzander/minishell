/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   htable_remove.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 12:31:58 by apierret          #+#    #+#             */
/*   Updated: 2025/06/17 13:55:51 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "hash.h"

int	htable_remove(t_hash_table *htable, char *key)
{
	size_t		key_len;
	size_t		index;
	t_hash_node	*node;
	t_list		*lst;
	t_list		*prev;

	if (htable == NULL || key == NULL)
		return (-1);
	key_len = ft_strlen(key);
	index = htable->hash(key) % htable->buckets_count;
	prev = NULL;
	lst = htable->buckets[index];
	while (lst != NULL)
	{
		node = lst->content;
		if (node != NULL && ft_strncmp(node->key, key, key_len +1) == 0)
		{
			if (prev != NULL)
				prev->next = lst->next;
			else
				htable->buckets[index] = lst->next;
			return (free_hnode(lst->content, htable->del), free(lst), 0);
		}
		prev = lst;
		lst = lst->next;
	}
	return (0);
}
