/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   htable_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:53:26 by apierret          #+#    #+#             */
/*   Updated: 2025/06/17 13:53:46 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hash.h"

t_hash_node	*htable_search_node(char *key, t_list *lst)
{
	size_t		key_len;
	t_hash_node	*node;

	key_len = ft_strlen(key);
	while (lst != NULL)
	{
		node = lst->content;
		if (node != NULL && ft_strncmp(node->key, key, key_len +1) == 0)
			return (node);
		lst = lst->next;
	}
	return (NULL);
}