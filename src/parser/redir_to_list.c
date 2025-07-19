/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_to_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 12:08:46 by apierret          #+#    #+#             */
/*   Updated: 2025/07/19 12:11:37 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_error	redir_to_list(t_list **lst, t_redir *redir)
{
	t_list	*node;

	if (lst == NULL || redir == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	node = ft_lstnew(redir);
	if (node == NULL)
		return (error(ERR_ALLOCATION, NULL));
	ft_lstadd_back(lst, node);
	return (error(ERR_NONE, NULL));
}
