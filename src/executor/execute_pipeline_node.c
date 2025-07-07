/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline_node.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:32:53 by apierret          #+#    #+#             */
/*   Updated: 2025/07/07 14:18:38 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "executor.h"
#include "utils.h"

static t_error	prepare_pids(t_ret **rets, int size)
{
	t_ret	*arr;
	int		i;

	if (rets == NULL || size < 1)
		return (error(ERR_IMPLEMENTATION, NULL));
	arr = ft_calloc(size, sizeof(t_ret));
	if (arr == NULL)
		return (error(ERR_ALLOCATION, NULL));
	i = 0;
	while (i < size)
	{
		arr[i].type = RET_PID;
		arr[i].pid = -1;
		i++;
	}
	return (*rets = arr, error(ERR_NONE, NULL));
}

static t_error	prepare_pipe_fds(t_list *node, t_pipe_fds *fds)
{
	t_ast	*cmd;
	t_error	err;

	if (node->next == NULL)
	{
		close_set(&fds->pipe[1], -1);
		fds->pipe[0] = -1;
	}
	else if (pipe(fds->pipe) == -1)
		return (error(ERR_PIPE, NULL));
	cmd = node->content;
	err = add_pipe_redir(&cmd->redirs, fds->input, fds->pipe[1],
			fds->pipe[0]);
	return (err);
}

static t_error	process_exec_pipeline(t_list *node, t_hash_table *env,
	t_pipe_fds *fds, t_ret *ret)
{
	t_ast	*cmd;
	t_error	err;

	if (node == NULL || env == NULL || fds == NULL || ret == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	ret->type = RET_PID;
	ret->pid = -1;
	err = error(ERR_NONE, NULL);
	cmd = node->content;
	if (cmd->type == NODE_COMMAND)
	{
		err = execute_command(ret, &cmd->command_args, cmd->redirs, env);
		if (ret->type == RET_PID)
			close_set(&fds->input, fds->pipe[0]);
		else
			close_fds(STDIN_FILENO, STDOUT_FILENO);
	}
	else if (cmd->type == NODE_REDIR)
		err = execute_redir_node(cmd, env);
	if (ret->type == RET_PID)
		close_set(&fds->pipe[1], -1);
	return (err);
}

static t_error	prepare_vars(t_ret **rets, t_pipe_fds *fds, int *i, int size)
{
	if (rets == NULL || fds == NULL || i == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	*i = 0;
	fds->input = -1;
	return (prepare_pids(rets, size));
}

t_error	execute_pipeline_node(t_ast *node, t_hash_table *env)
{
	t_list		*lst;
	t_pipe_fds	fds;
	t_ret		*rets;
	int			i;
	t_error		err;

	if (node == NULL || env == NULL || node->type != NODE_PIPELINE)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = prepare_vars(&rets, &fds, &i, ft_lstsize(node->pipeline));
	if (err.id != ERR_NONE)
		return (err);
	lst = node->pipeline;
	while (lst != NULL && err.id != ERR_EXIT)
	{
		err = prepare_pipe_fds(lst, &fds);
		if (err.id != ERR_NONE)
			return (close_fd(fds.input), close_pipe(fds.pipe), free(rets), err);
		err = process_exec_pipeline(lst, env, &fds, rets + i++);
		if (lst->next != NULL && err.id != ERR_EXIT)
			print_error(err);
		lst = lst->next;
	}
	node->exit_code = get_exit_code_pipe(rets, ft_lstsize(node->pipeline), err);
	return (free(rets), err);
}
