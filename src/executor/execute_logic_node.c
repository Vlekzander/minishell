/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_logic_node.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:32:53 by apierret          #+#    #+#             */
/*   Updated: 2025/07/10 00:33:44 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

t_error	execute_logic_node(t_ast *node, t_hash_table *env)
{
	t_error	err;

	if (node == NULL || env == NULL
		|| (node->type != NODE_AND && node->type != NODE_OR))
		return (error(ERR_IMPLEMENTATION, NULL));
	err = execute_node(node->left, env);
	node->exit_code = node->left->exit_code;
	if (err.id != ERR_NONE)
		return (err);
	if ((node->type == NODE_AND && node->exit_code != 0)
		|| (node->type == NODE_OR && node->exit_code == 0))
		return (err);
	err = execute_node(node->right, env);
	node->exit_code = node->right->exit_code;
	return (err);
}
