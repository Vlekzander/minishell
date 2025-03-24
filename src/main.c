/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 17:49:01 by apierret          #+#    #+#             */
/*   Updated: 2025/03/24 15:39:09 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <readline/readline.h>

int	main(void)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (line != NULL)
		{
			free(line);
		}
	}
	return (0);
}
