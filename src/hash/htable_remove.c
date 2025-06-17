/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   htable_remove.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 12:31:58 by apierret          #+#    #+#             */
/*   Updated: 2025/06/17 14:08:12 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "hash.h"

static t_list	*find_deleted_node(t_list **head, char *key)
{
	size_t		key_len;
	t_list		*lst;
	t_list		*prev;
	t_hash_node	*node;

	key_len = ft_strlen(key);
	prev = NULL;
	lst = *head;
	while (lst != NULL)
	{
		node = lst->content;
		if (node != NULL && ft_strncmp(node->key, key, key_len +1) == 0)
		{
			if (prev != NULL)
				prev->next = lst->next;
			else
				*head = lst->next;
			return (lst);
		}
		prev = lst;
		lst = lst->next;
	}
	return (NULL);
}

int	htable_remove(t_hash_table *htable, char *key)
{
	size_t	index;
	t_list	*node;

	if (htable == NULL || key == NULL)
		return (-1);
	index = htable->hash(key) % htable->buckets_count;
	node = find_deleted_node(&htable->buckets[index], key);
	if (node != NULL)
	{
		free_hnode(node->content, htable->del);
		free(node);
	}
	return (0);
}
