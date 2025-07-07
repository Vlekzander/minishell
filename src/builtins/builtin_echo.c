/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:02:26 by apierret          #+#    #+#             */
/*   Updated: 2025/07/07 18:15:10 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

t_error	builtin_echo(int *ret, t_btin_data data, t_hash_table *env)
{
	int	i;
	int	new_line;

	if (ret == NULL || data.argv == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	new_line = 1;
	i = 1;
	while (i < data.argc)
	{
		if (i == 1 && ft_strncmp(data.argv[i], "-n", 3) == 0)
			new_line = 0;
		else
		{
			ft_putstr_fd(data.argv[i], data.stdout);
			if (i != data.argc -1)
				ft_putchar_fd(' ', data.stdout);
		}
		i++;
	}
	if (new_line)
		ft_putchar_fd('\n', data.stdout);
	return (*ret = 0, error(ERR_NONE, NULL));
}
