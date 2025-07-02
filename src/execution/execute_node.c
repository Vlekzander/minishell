/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:09:08 by apierret          #+#    #+#             */
/*   Updated: 2025/06/24 18:32:12 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

t_error	execute_node(t_ast *node, t_hash_table *env)
{
	t_error	err;

	if (node == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = error(ERR_NONE, NULL);
	if (node->type == NODE_GROUP)
	{
		err = execute_node(node->group, env);
		if (err.code != ERR_NONE)
			return (err);
		node->exit_code = node->group->exit_code;
	}
	else if (node->type == NODE_COMMAND)
		err = execute_cmd_node(node, env);
	else if (node->type == NODE_PIPELINE)
		err = execute_pipeline_node(node, env);
	else if (node->type == NODE_AND || node->type == NODE_OR)
		err = execute_logic_node(node, env);
	else if (node->type == NODE_REDIR)
		err = execute_redir_node(node, env);
	return (err);
}
