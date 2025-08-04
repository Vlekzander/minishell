/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_redirs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:13:04 by apierret          #+#    #+#             */
/*   Updated: 2025/08/03 18:50:43 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "data.h"
#include "error.h"
#include "expansion.h"
#include "libft.h"
#include "redirs.h"
#include "utils.h"

static t_error	prepare_redir_in(t_redir *redir)
{
	t_error	err;

	if (redir == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = check_file(redir->in, 1, 1, 0);
	if (err.id != ERR_NONE)
		return (err);
	err = open_file(&redir->fd, redir->in, redir->type, 0);
	if (err.id != ERR_NONE)
		return (err);
	return (error(ERR_NONE, NULL));
}

static t_error	prepare_redir_out(t_redir *redir)
{
	t_error	err;

	if (redir == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = check_file(redir->out, 1, 0, 1);
	if (err.id != ERR_NONE)
		return (err);
	err = open_file(&redir->fd, redir->out, redir->type, redir->append);
	if (err.id != ERR_NONE)
		return (err);
	return (error(ERR_NONE, NULL));
}

t_error	prepare_redirs(t_list *redirs, t_hash_table *env)
{
	t_redir	*redir;
	t_error	err;

	while (redirs != NULL)
	{
		redir = redirs->content;
		if (redir->type == REDIR_HEREDOC || redir->type == REDIR_PIPE)
			err = error(ERR_NONE, NULL);
		else if (redir->type == REDIR_IN)
			err = expand_redir_target(&redir->in, env);
		else
			err = expand_redir_target(&redir->out, env);
		if (err.id != ERR_NONE)
			return (err);
		if (redir->type == REDIR_IN)
			err = prepare_redir_in(redir);
		else if (redir->type == REDIR_OUT)
			err = prepare_redir_out(redir);
		if (err.id != ERR_NONE)
			return (err);
		redirs = redirs->next;
	}
	return (error(ERR_NONE, NULL));
}
