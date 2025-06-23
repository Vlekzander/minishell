/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 19:18:31 by apierret          #+#    #+#             */
/*   Updated: 2025/06/23 23:07:22 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "expand.h"
#include "redirs.h"

static t_error	process_redir_in(t_redir *redir)
{
	int		fd;
	t_error	err;

	if (redir == NULL || redir->type != REDIR_IN || redir->in == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = open_file(&fd, redir->in, redir->type, 0);
	if (err.code != ERR_NONE)
		return (err);
	dup2(fd, STDIN_FILENO);
	return (close(fd), error(ERR_NONE, NULL));
}

static t_error	process_redir_out(t_redir *redir)
{
	int		fd;
	t_error	err;

	if (redir == NULL || redir->type != REDIR_OUT || redir->out == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = open_file(&fd, redir->out, redir->type, redir->append);
	if (err.code != ERR_NONE)
		return (err);
	dup2(fd, STDOUT_FILENO);
	return (close(fd), error(ERR_NONE, NULL));
}

static t_error	process_redir_heredoc(t_redir *redir)
{
	if (redir == NULL || redir->type != REDIR_HEREDOC)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (redir->fd == -1)
		return (error(ERR_NONE, NULL));
	dup2(redir->fd, STDIN_FILENO);
	return (close(redir->fd), error(ERR_NONE, NULL));
}

t_error	handle_redirs(t_list *redirs, t_hash_table *env)
{
	t_redir	*redir;
	t_error	err;

	while (redirs != NULL)
	{
		redir = redirs->content;
		err = expand_redir_target(redir, env);
		if (err.code != ERR_NONE)
			return (err);
		if (redir->type == REDIR_IN)
			err = process_redir_in(redir);
		else if (redir->type == REDIR_OUT)
			err = process_redir_out(redir);
		else if (redir->type == REDIR_HEREDOC)
			err = process_redir_heredoc(redir);
		if (err.code != ERR_NONE)
			return (err);
		redirs = redirs->next;
	}
	return (error(ERR_NONE, NULL));
}
