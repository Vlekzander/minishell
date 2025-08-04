/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 14:30:36 by apierret          #+#    #+#             */
/*   Updated: 2025/08/04 19:19:08 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "data.h"
#include "utils.h"

void	free_command(t_command *command)
{
	if (command == NULL)
		return ;
	free_ddarray((void **) command->argv);
	free_ddarray((void **) command->envp);
	if (command->type == CMD_BINARY)
		free(command->executable);
	free(command);
}
