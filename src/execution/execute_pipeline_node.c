/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline_node.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:32:53 by apierret          #+#    #+#             */
/*   Updated: 2025/07/07 12:44:15 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "execution.h"
#include "utils.h"

static t_error	add_pipe_redir(t_list **redirs, int in, int out, int close)
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
	redir->fd_close = close;
	node = ft_lstnew(redir);
	if (node == NULL)
		return (free_redir(redir), error(ERR_ALLOCATION, NULL));
	ft_lstadd_front(redirs, node);
	return (error(ERR_NONE, NULL));
}

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

static t_error	prepare_pipe_fds(t_list *node, t_ast **cmd, t_pipe_fds *fds)
{
	t_error	err;

	if (node->next == NULL)
	{
		close_set(&fds->pipe[1], -1);
		fds->pipe[0] = -1;
	}
	else if (pipe(fds->pipe) == -1)
		return (error(ERR_PIPE, NULL));
	*cmd = node->content;
	err = add_pipe_redir(&(*cmd)->redirs, fds->input, fds->pipe[1],
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
	err = prepare_pipe_fds(node, &cmd, fds);
	if (err.id != ERR_NONE)
		return (err);
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
	if (err.id != ERR_NONE && err.id != ERR_EXIT)
		return (print_error(err), error(ERR_NONE, NULL));
	return (err);
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
	fds.input = -1;
	err = prepare_pids(&rets, ft_lstsize(node->pipeline));
	if (err.id != ERR_NONE)
		return (err);
	i = 0;
	lst = node->pipeline;
	while (lst != NULL)
	{
		err = process_exec_pipeline(lst, env, &fds, rets + i++);
		if (err.id == ERR_EXIT)
			break ;
		if (err.id != ERR_NONE)
			return (close_fd(fds.input), close_pipe(fds.pipe), free(rets), err);
		lst = lst->next;
	}
	node->exit_code = get_exit_code_pipe(rets, ft_lstsize(node->pipeline), err);
	return (free(rets), err);
}
