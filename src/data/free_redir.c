/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 22:50:02 by apierret          #+#    #+#             */
/*   Updated: 2025/07/04 22:10:54 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "data.h"
#include "utils.h"

void	free_redir(t_redir *redir)
{
	if (redir == NULL)
		return ;
	if (redir->type == REDIR_IN)
		free(redir->in);
	else if (redir->type == REDIR_OUT)
		free(redir->out);
	else if (redir->type == REDIR_HEREDOC)
	{
		free(redir->heredoc);
		close_fd(redir->fd);
	}
	free(redir);
}
