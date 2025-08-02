/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redir_node.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:32:53 by apierret          #+#    #+#             */
/*   Updated: 2025/08/02 13:44:07 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

t_error	execute_redir_node(t_ast *node, t_hash_table *env)
{
	if (node == NULL || env == NULL || node->type != NODE_REDIR)
		return (error(ERR_IMPLEMENTATION, NULL));
	return (error(ERR_NONE, NULL));
}
