/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   in_pipeline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:34:01 by apierret          #+#    #+#             */
/*   Updated: 2025/08/04 14:34:34 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"
#include "executor.h"
#include "libft.h"

int	in_pipeline(t_list *redirs)
{
	t_redir	*redir;

	if (redirs == NULL)
		return (0);
	redir = redirs->content;
	if (redir->type == REDIR_PIPE)
		return (1);
	return (0);
}
