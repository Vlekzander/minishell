/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   htable_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 22:42:24 by apierret          #+#    #+#             */
/*   Updated: 2025/06/17 23:42:21 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "hash.h"

t_hash_table	*htable_init(size_t buckets_count, size_t (*hash)(char *key),
					void (*del)(void *))
{
	t_hash_table	*htable;

	if (buckets_count == 0)
		return (NULL);
	htable = ft_calloc(1, sizeof(t_hash_table));
	if (htable == NULL)
		return (NULL);
	htable->buckets = ft_calloc(buckets_count, sizeof(t_list *));
	if (htable->buckets == NULL)
		return (free(htable), NULL);
	htable->buckets_count = buckets_count;
	htable->hash = htable_hash;
	if (hash != NULL)
		htable->hash = hash;
	htable->del = del;
	return (htable);
}
