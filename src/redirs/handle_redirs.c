/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 19:18:31 by apierret          #+#    #+#             */
/*   Updated: 2025/06/06 19:20:53 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirs.h"

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
		redirs = redirs->next;
	}
	return (error);
}
