/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_hnode.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:56:17 by apierret          #+#    #+#             */
/*   Updated: 2025/06/17 13:56:49 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "hash.h"

t_hash_node	*create_hnode(char *key, void *value)
{
	t_hash_node	*hnode;

	if (key == NULL || value == NULL)
		return (NULL);
	hnode = ft_calloc(1, sizeof(t_hash_node));
	if (hnode == NULL)
		return (NULL);
	hnode->key = ft_strdup(key);
	if (hnode->key == NULL)
		return (free(hnode), NULL);
	hnode->content = value;
	return (hnode);
}
