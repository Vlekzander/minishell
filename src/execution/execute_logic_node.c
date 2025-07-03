/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_logic_node.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:32:53 by apierret          #+#    #+#             */
/*   Updated: 2025/07/03 23:46:13 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

t_error	execute_logic_node(t_ast *node, t_hash_table *env)
{
	t_error	err;

	if (node == NULL || env == NULL
		|| (node->type != NODE_AND && node->type != NODE_OR))
		return (error(ERR_IMPLEMENTATION, NULL));
	err = execute_node(node->left, env);
	if (err.id != ERR_NONE)
		return (err);
	if ((node->type == NODE_AND && node->left->exit_code != 0)
		|| (node->type == NODE_OR && node->left->exit_code == 0))
		return (node->exit_code = node->left->exit_code, err);
	err = execute_node(node->right, env);
	if (err.id != ERR_NONE)
		return (err);
	node->exit_code = node->right->exit_code;
	return (error(ERR_NONE, NULL));
}
