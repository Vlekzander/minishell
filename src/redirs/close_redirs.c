/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_redirs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 21:35:23 by apierret          #+#    #+#             */
/*   Updated: 2025/07/16 21:36:56 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"
#include "libft.h"
#include "redirs.h"
#include "utils.h"

void	close_redirs(t_list *redirs)
{
	t_redir	*redir;

	while (redirs != NULL)
	{
		redir = redirs->content;
		if (redir->type == REDIR_IN || redir->type == REDIR_OUT)
			close_fd(redir->fd);
		redirs = redirs->next;
	}
}
