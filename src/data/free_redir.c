/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 22:50:02 by apierret          #+#    #+#             */
/*   Updated: 2025/06/09 16:34:03 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "data.h"

void	free_redir(t_redir *redir)
{
	if (redir == NULL)
		return ;
	if (redir->type == REDIR_IN)
		free(redir->in);
	if (redir->type == REDIR_OUT)
		free(redir->out);
	if (redir->type == REDIR_HEREDOC)
	{
		free(redir->heredoc);
		if (redir->fd != -1)
			close(redir->fd);
	}
	free(redir);
}
