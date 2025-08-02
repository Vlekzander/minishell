/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:09:08 by apierret          #+#    #+#             */
/*   Updated: 2025/08/02 13:45:03 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "env.h"
#include "executor.h"

t_error	execute_node(t_ast *node, t_hash_table *env)
{
	char	*str;
	t_error	err;

	if (node == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = error(ERR_NONE, NULL);
	if (node->type == NODE_GROUP)
	{
		err = execute_node(node->group, env);
		node->exit_code = node->group->exit_code;
	}
	else if (node->type == NODE_COMMAND)
		err = execute_command_node(node, env);
	else if (node->type == NODE_PIPELINE)
		err = execute_pipeline_node(node, env);
	else if (node->type == NODE_AND || node->type == NODE_OR)
		err = execute_logic_node(node, env);
	else if (node->type == NODE_REDIR)
		err = execute_redir_node(node, env);
	str = ft_itoa(node->exit_code);
	if (str == NULL)
		return (error(ERR_ALLOCATION, NULL));
	return (set_var(env, "?", str), free(str), err);
}
