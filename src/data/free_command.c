/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:04:41 by apierret          #+#    #+#             */
/*   Updated: 2025/04/04 15:06:10 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "data.h"

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
	if (command->redir.in != NULL)
		free(command->redir.in);
	if (command->redir.out != NULL)
		free(command->redir.out);
	if (command->redir.heredoc != NULL)
		free(command->redir.heredoc);
	free(command);
}
