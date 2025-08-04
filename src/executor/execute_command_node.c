/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_node.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 14:32:02 by apierret          #+#    #+#             */
/*   Updated: 2025/08/04 14:30:38 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "executor.h"
#include "expansion.h"

t_error	execute_command_node(t_ast *node, t_hash_table *env)
{
	t_ret	ret;
	t_error	err;

	if (node == NULL || env == NULL || node->type != NODE_COMMAND)
		return (error(ERR_IMPLEMENTATION, NULL));
	ret = (t_ret){0};
	signal(SIGINT, SIG_IGN);
	err = execute_command(&ret, &node->command_args, node->redirs, env);
	node->exit_code = get_exit_code(ret, err);
	if (ret.ret_sub != RET_SUB_NONE)
		err.exit = 1;
	return (err);
}
