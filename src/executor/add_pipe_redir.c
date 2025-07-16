/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_pipe_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:05:24 by apierret          #+#    #+#             */
/*   Updated: 2025/07/07 14:05:37 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

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
