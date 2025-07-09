/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_node.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 14:32:02 by apierret          #+#    #+#             */
/*   Updated: 2025/07/09 17:02:12 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include "executor.h"
#include "signals.h"

t_error	execute_command_node(t_ast *node, t_hash_table *env)
{
	t_ret	ret;
	t_error	err;

	if (node == NULL || env == NULL || node->type != NODE_COMMAND)
		return (error(ERR_IMPLEMENTATION, NULL));
	ret.type = RET_VALUE;
	ret.value = 0;
	ret.redir_error = 0;
	err = execute_command(&ret, &node->command_args, node->redirs, env);
	ignore_signal();
	node->exit_code = get_exit_code(ret, err);
	return (err);
}
