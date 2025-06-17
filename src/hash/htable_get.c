/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   htable_get.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 12:31:13 by apierret          #+#    #+#             */
/*   Updated: 2025/06/17 13:57:00 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hash.h"

void	*htable_get(t_hash_table *htable, char *key)
{
	size_t		index;
	t_hash_node	*node;

	if (htable == NULL || key == NULL)
		return (NULL);
	index = htable->hash(key) % htable->buckets_count;
	node = htable_search_node(key, htable->buckets[index]);
	if (node == NULL)
		return (NULL);
	return (node->content);
}
