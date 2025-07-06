/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 19:18:31 by apierret          #+#    #+#             */
/*   Updated: 2025/07/06 21:14:20 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "expand.h"
#include "redirs.h"
#include "utils.h"

static t_error	process_redir_in(t_redir *redir, int in)
{
	int		fd;
	int		ret;
	t_error	err;

	if (redir == NULL || redir->type != REDIR_IN || redir->in == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = open_file(&fd, redir->in, redir->type, 0);
	if (err.id != ERR_NONE)
		return (err);
	ret = 0;
	if (in != -1)
		ret = dup2(fd, in);
	if (ret == -1)
		err = error(ERR_ERRNO, NULL);
	return (close_fd(fd), err);
}

static t_error	process_redir_out(t_redir *redir, int out)
{
	int		fd;
	int		ret;
	t_error	err;

	if (redir == NULL || redir->type != REDIR_OUT || redir->out == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = open_file(&fd, redir->out, redir->type, redir->append);
	if (err.id != ERR_NONE)
		return (err);
	ret = 0;
	if (out != -1)
		ret = dup2(fd, out);
	if (ret == -1)
		err = error(ERR_ERRNO, NULL);
	return (close_fd(fd), err);
}

static t_error	process_redir_heredoc(t_redir *redir, int in)
{
	int		ret;
	t_error	err;

	if (redir == NULL || redir->type != REDIR_HEREDOC)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (redir->fd == -1)
		return (error(ERR_NONE, NULL));
	ret = 0;
	err = error(ERR_NONE, NULL);
	if (in != -1)
		ret = dup2(redir->fd, in);
	if (ret == -1)
		err = error(ERR_ERRNO, NULL);
	return (close_fd(redir->fd), err);
}

static t_error	process_redir_pipe(t_redir *redir, int in, int out)
{
	int		ret;
	t_error	err;

	if (redir == NULL || redir->type != REDIR_PIPE)
		return (error(ERR_IMPLEMENTATION, NULL));
	ret = 0;
	err = error(ERR_NONE, NULL);
	if (in != -1 && out != -1)
	{
		if (redir->pipe_fds[0] != -1)
			ret = dup2(redir->pipe_fds[0], in);
		if (ret != -1 && redir->pipe_fds[1] != -1)
			dup2(redir->pipe_fds[1], out);
	}
	if (ret == -1)
		err = error(ERR_ERRNO, NULL);
	close_set(&redir->pipe_fds[0], -1);
	close_set(&redir->pipe_fds[1], -1);
	close_set(&redir->fd_close, -1);
	return (err);
}

t_error	handle_redirs(t_list *redirs, int in, int out)
{
	t_redir	*redir;
	t_error	err;

	while (redirs != NULL)
	{
		redir = redirs->content;
		err = error(ERR_NONE, NULL);
		if (redir->type == REDIR_IN)
			err = process_redir_in(redir, in);
		else if (redir->type == REDIR_OUT)
			err = process_redir_out(redir, out);
		else if (redir->type == REDIR_HEREDOC)
			err = process_redir_heredoc(redir, in);
		else if (redir->type == REDIR_PIPE)
			err = process_redir_pipe(redir, in, out);
		if (err.id != ERR_NONE)
			return (err);
		redirs = redirs->next;
	}
	return (error(ERR_NONE, NULL));
}
