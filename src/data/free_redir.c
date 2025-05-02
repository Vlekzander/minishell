/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 22:50:02 by apierret          #+#    #+#             */
/*   Updated: 2025/05/01 22:50:40 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
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
		free(redir->heredoc);
	free(redir);
}
