/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redir_target.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:53:58 by apierret          #+#    #+#             */
/*   Updated: 2025/08/04 21:01:45 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "expansion.h"
#include "lexer.h"

static t_error	create_blank_node(t_list **node)
{
	t_list	*new_node;
	char	*str;

	if (node == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	str = ft_strdup("");
	if (str == NULL)
		return (error(ERR_ALLOCATION, NULL));
	new_node = ft_lstnew(str);
	if (new_node == NULL)
		return (error(ERR_ALLOCATION, NULL));
	return (*node = new_node, error(ERR_NONE, NULL));
}

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
	if (node == NULL)
	{
		err = create_blank_node(&node);
		if (err.id != ERR_NONE)
			return (err);
	}
	if (node->content == NULL || node->next != NULL)
		return (ft_lstclear(&node, free), error(ERR_AMB_REDIR, *target));
	free(*target);
	return (*target = node->content, free(node), error(ERR_NONE, NULL));
}
