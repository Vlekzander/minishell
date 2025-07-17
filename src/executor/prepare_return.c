/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_return.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 16:11:21 by apierret          #+#    #+#             */
/*   Updated: 2025/07/17 16:17:55 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

t_error	prepare_return(t_ret *ret, t_cmd_type type, t_list *redirs)
{
	if (ret == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	ret->type = RET_VALUE;
	ret->value = 1;
	if (type == CMD_BINARY
		|| (type == CMD_BUILTIN && redirs != NULL
			&& redirs->content != NULL
			&& ((t_redir *) redirs->content)->type == REDIR_PIPE))
	{
		ret->type = RET_PID;
		ret->pid = -1;
	}
	return (error(ERR_NONE, NULL));
}
