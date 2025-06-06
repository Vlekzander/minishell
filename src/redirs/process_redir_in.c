/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_redir_in.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 19:18:31 by apierret          #+#    #+#             */
/*   Updated: 2025/06/06 19:26:58 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "redirs.h"

t_error	process_redir_in(t_redir *redir)
{
	int		fd;
	t_error	error;

	if (redir == NULL || redir->type != REDIR_IN || redir->in == NULL)
		return (ERR_IMPLEMENTATION);
	error = open_file(&fd, redir->in, 0, 0);
	if (error != ERR_NONE)
		return (error);
	dup2(fd, STDIN_FILENO);
	return (close(fd), ERR_NONE);
}
