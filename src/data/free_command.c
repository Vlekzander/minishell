/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:04:41 by apierret          #+#    #+#             */
/*   Updated: 2025/04/09 13:48:26 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "data.h"

static void	free_redir(t_redir	*redir)
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

void	free_command(t_command *command)
{
	int	i;

	if (command == NULL)
		return ;
	free(command->path);
	if (command->args != NULL)
	{
		i = 0;
		while (command->args[i] != NULL)
			free(command->args[i++]);
		free(command->args);
	}
	ft_lstclear(&command->redirs, (void (*)(void *)) free_redir);
	free(command);
}
