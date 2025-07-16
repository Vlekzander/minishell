/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:02:26 by apierret          #+#    #+#             */
/*   Updated: 2025/07/16 22:06:55 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static int	new_line_option(char *str)
{
	int	i;

	if (str == NULL)
		return (0);
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (i > 1);
}

static int	process_setup(char *str, int *new_line)
{
	if (str == NULL || new_line == NULL)
		return (0);
	if (str[0] == '-' && new_line_option(str + 1))
		return (*new_line = 0, 1);
	return (0);
}

t_error	builtin_echo(int *ret, t_btin_data data, t_hash_table *env)
{
	int	i;
	int	new_line;
	int	setup;

	if (ret == NULL || data.argv == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	setup = 1;
	new_line = 1;
	i = 1;
	while (i < data.argc)
	{
		if (setup)
			setup = process_setup(data.argv[i], &new_line);
		if (!setup)
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
