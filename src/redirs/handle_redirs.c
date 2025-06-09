/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 19:18:31 by apierret          #+#    #+#             */
/*   Updated: 2025/06/09 17:04:54 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "redirs.h"

static t_error	process_redir_in(t_redir *redir)
{
	int		fd;
	t_error	error;

	if (redir == NULL || redir->type != REDIR_IN || redir->in == NULL)
		return (ERR_IMPLEMENTATION);
	error = open_file(&fd, redir->in, redir->type, 0);
	if (error != ERR_NONE)
		return (error);
	dup2(fd, STDIN_FILENO);
	return (close(fd), ERR_NONE);
}

static t_error	process_redir_out(t_redir *redir)
{
	int		fd;
	t_error	error;

	if (redir == NULL || redir->type != REDIR_OUT || redir->out == NULL)
		return (ERR_IMPLEMENTATION);
	error = open_file(&fd, redir->out, redir->type, redir->append);
	if (error != ERR_NONE)
		return (error);
	dup2(fd, STDOUT_FILENO);
	return (close(fd), ERR_NONE);
}

static t_error	process_redir_heredoc(t_redir *redir)
{
	if (redir == NULL || redir->type != REDIR_HEREDOC)
		return (ERR_IMPLEMENTATION);
	if (redir->fd == -1)
		return (ERR_NONE);
	dup2(redir->fd, STDIN_FILENO);
	return (close(redir->fd), ERR_NONE);
}

t_error	handle_redirs(t_list *redirs)
{
	t_redir	*redir;
	t_error	error;

	error = ERR_NONE;
	while (redirs != NULL)
	{
		redir = redirs->content;
		if (redir->type == REDIR_IN)
			error = process_redir_in(redir);
		else if (redir->type == REDIR_OUT)
			error = process_redir_out(redir);
		else if (redir->type == REDIR_HEREDOC)
			error = process_redir_heredoc(redir);
		redirs = redirs->next;
	}
	return (error);
}
