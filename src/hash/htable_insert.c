/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   htable_insert.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 12:07:12 by apierret          #+#    #+#             */
/*   Updated: 2025/07/08 18:44:09 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "hash.h"

int	htable_insert(t_hash_table *htable, char *key, void *value)
{
	size_t		index;
	t_list		*lst;
	t_hash_node	*node;

	if (htable == NULL || key == NULL)
		return (-1);
	index = htable->hash(key) % htable->buckets_count;
	node = htable_search_node(key, htable->buckets[index]);
	if (node == NULL)
	{
		node = create_hnode(key, value);
		if (node == NULL)
			return (-1);
		lst = ft_lstnew(node);
		if (lst == NULL)
			return (free(node->key), free(node), -1);
		ft_lstadd_back(&htable->buckets[index], lst);
	}
	else if (htable->del != NULL)
		htable->del(node->content);
	node->content = value;
	return (0);
}
