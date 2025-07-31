/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redir_target.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:53:58 by apierret          #+#    #+#             */
/*   Updated: 2025/07/31 17:49:10 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "expansion.h"

t_error	expand_redir_target(char **target, t_hash_table *env)
{
	t_list	*node;
	char	*content;
	t_error	err;

	if (target == NULL || *target == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	content = ft_strdup(*target);
	if (content == NULL)
		return (error(ERR_ALLOCATION, NULL));
	node = ft_lstnew(content);
	if (node == NULL)
		return (free(content), error(ERR_ALLOCATION, NULL));
	err = expand_list(&node, env);
	if (err.id != ERR_NONE)
		return (ft_lstclear(&node, free), err);
	if (node->next != NULL || ft_strncmp("", node->content, 1) == 0)
		return (ft_lstclear(&node, free), error(ERR_AMB_REDIR, *target));
	free(*target);
	*target = node->content;
	free(node);
	return (error(ERR_NONE, NULL));
}
