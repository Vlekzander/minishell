/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_node.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 14:32:02 by apierret          #+#    #+#             */
/*   Updated: 2025/08/02 17:41:09 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "executor.h"
#include "expansion.h"
#include "redirs.h"

static t_error	exec_fork(t_ret *ret, t_list *args, t_list *redirs,
					t_hash_table *env)
{
	pid_t		pid;

	if (ret == NULL || args == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	ret->type = RET_PID;
	pid = fork();
	if (pid == -1)
		return (ret->pid = -1, error(ERR_FORK, NULL));
	if (pid == 0)
		return (execute_command(ret, args, redirs, env));
	ret->pid = pid;
	return (error(ERR_NONE, NULL));
}

t_error	exec_cmd(t_ret *ret, t_list **args, t_list *redirs, t_hash_table *env)
{
	t_error	err;

	if (ret == NULL || args == NULL || *args == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = prepare_redirs(redirs, env);
	if (err.id != ERR_NONE)
		return (close_redirs(redirs), err);
	err = expand_list(args, env);
	if (err.id != ERR_NONE)
		return (close_redirs(redirs), err);
	if (get_builtin((*args)->content) != NULL)
	{
		err = execute_command(ret, *args, redirs, env);
		return (close_redirs(redirs), err);
	}
	err = exec_fork(ret, *args, redirs, env);
	if (err.id != ERR_NONE)
		return (close_redirs(redirs), err);
	return (close_redirs(redirs), error(ERR_NONE, NULL));
}

t_error	execute_command_node(t_ast *node, t_hash_table *env)
{
	t_ret	ret;
	t_error	err;

	if (node == NULL || env == NULL || node->type != NODE_COMMAND)
		return (error(ERR_IMPLEMENTATION, NULL));
	ret = (t_ret){0};
	err = exec_cmd(&ret, &node->command_args, node->redirs, env);
	node->exit_code = get_exit_code(ret, err);
	if (ret.ret_sub != RET_SUB_NONE)
		return (print_error(err, NULL), error(ERR_EXIT, NULL));
	return (err);
}
