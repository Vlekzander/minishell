/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 17:49:01 by apierret          #+#    #+#             */
/*   Updated: 2025/06/07 17:11:30 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <readline/history.h>
#include <readline/readline.h>
#include "libft.h"
#include "signals.h"

int	main(void)
{
	char	*line;
	int		run;
	int		ret;

	setup_signals();
	ret = 0;
	run = 1;
	while (run)
	{
		line = readline("> ");
		if (line != NULL)
		{
			if (ft_strlen(line) != 0)
				add_history(line);
			if (ft_strncmp(line, "exit", 5) == 0)
				run = 0;
			free(line);
		}
		else
			run = 0;
	}
	return (ret);
}
