/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_hnode.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:55:16 by apierret          #+#    #+#             */
/*   Updated: 2025/06/17 13:56:04 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "hash.h"

void	free_hnode(t_hash_node *hnode, void (*del)(void *))
{
	if (hnode == NULL)
		return ;
	if (del != NULL)
		del(hnode->content);
	free(hnode->key);
	free(hnode);
}
