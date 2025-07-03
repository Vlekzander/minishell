/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_redirs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:13:04 by apierret          #+#    #+#             */
/*   Updated: 2025/07/03 17:26:34 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "data.h"
#include "error.h"
#include "expand.h"
#include "libft.h"
#include "redirs.h"

t_error	prepare_redirs(t_list *redirs, t_hash_table *env)
{
	t_redir	*redir;
	t_error	err;

	while (redirs != NULL)
	{
		redir = redirs->content;
		err = expand_redir_target(redir, env);
		if (err.id != ERR_NONE)
			return (err);
		if (redir->type == REDIR_IN)
			err = check_file(redir->in, REDIR_IN);
		else if (redir->type == REDIR_OUT)
			err = check_file(redir->in, REDIR_OUT);
		if (err.id != ERR_NONE)
			return (err);
		redirs = redirs->next;
	}
	return (error(ERR_NONE, NULL));
}
