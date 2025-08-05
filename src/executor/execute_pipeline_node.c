/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline_node.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:32:53 by apierret          #+#    #+#             */
/*   Updated: 2025/08/05 14:31:19 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdlib.h>
#include "executor.h"
#include "utils.h"

t_error	add_pipe_redir(t_list **redirs, int in, int out, int close)
{
	t_redir	*redir;
	t_list	*node;

	if (redirs == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	redir = ft_calloc(1, sizeof(t_redir));
	if (redir == NULL)
		return (error(ERR_ALLOCATION, NULL));
	redir->type = REDIR_PIPE;
	redir->pipe_fds[0] = in;
	redir->pipe_fds[1] = out;
	redir->fd = close;
	node = ft_lstnew(redir);
	if (node == NULL)
		return (free_redir(redir), error(ERR_ALLOCATION, NULL));
	ft_lstadd_front(redirs, node);
	return (error(ERR_NONE, NULL));
}

static t_error	prepare_pipe_fds(t_list *node, int *fds)
{
	t_ast	*cmd;
	int		*pipe_fds;
	t_error	err;

	pipe_fds = fds +1;
	if (node->next == NULL)
	{
		close_set(&pipe_fds[1], -1);
		pipe_fds[0] = -1;
	}
	else if (pipe(pipe_fds) == -1)
		return (error(ERR_PIPE, NULL));
	cmd = node->content;
	if (cmd->type != NODE_COMMAND)
		return (error(ERR_NONE, NULL));
	err = add_pipe_redir(&cmd->redirs, fds[0], pipe_fds[1],
			pipe_fds[0]);
	return (err);
}

static t_error	prepare_vars(t_ret **rets, int *fds, int *index, int size)
{
	t_ret	*arr;
	int		i;

	if (rets == NULL || fds == NULL || index == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	*index = 0;
	fds[0] = -1;
	arr = ft_calloc(size, sizeof(t_ret));
	if (arr == NULL)
		return (error(ERR_ALLOCATION, NULL));
	i = 0;
	while (i < size)
	{
		arr[i] = (t_ret){0};
		arr[i].pid = -1;
		i++;
	}
	return (signal(SIGINT, SIG_IGN), *rets = arr, error(ERR_NONE, NULL));
}

t_error	process_exec_pipeline(t_ret *ret, t_ast *node, int *fds,
			t_hash_table *env)
{
	int		*pipe_fds;
	t_error	err;

	if (ret == NULL || node == NULL || fds == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	pipe_fds = fds + 1;
	err = error(ERR_NONE, NULL);
	if (node->type == NODE_COMMAND)
	{
		err = execute_command(ret, &node->command_args, node->redirs, env);
		if (!err.exit)
			close_set(&fds[0], pipe_fds[0]);
		if (ret->ret_sub != RET_SUB_NONE)
			close_set(&fds[0], -1);
	}
	else if (node->type == NODE_REDIR)
	{
		err = execute_redir_node(node, env);
		close_set(&fds[0], pipe_fds[0]);
	}
	if (!err.exit)
		close_set(&pipe_fds[1], -1);
	return (err);
}

t_error	execute_pipeline_node(t_ast *node, t_hash_table *env)
{
	t_list	*lst;
	t_ret	*rets;
	int		i;
	int		fds[3];
	t_error	err;

	if (node == NULL || env == NULL || node->type != NODE_PIPELINE)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = prepare_vars(&rets, fds, &i, ft_lstsize(node->pipeline));
	if (err.id != ERR_NONE)
		return (err);
	lst = node->pipeline;
	while (lst != NULL && !err.exit)
	{
		err = prepare_pipe_fds(lst, fds);
		if (err.id != ERR_NONE)
			return (close_fd(fds[0]), close_pipe(fds +1), free(rets), err);
		err = process_exec_pipeline(&rets[i], lst->content, fds, env);
		if (rets[i].ret_sub != RET_SUB_NONE)
			err.exit = 1;
		i++;
		lst = lst->next;
	}
	node->exit_code = get_exit_code_pipe(rets, i, err);
	return (free(rets), err);
}
