/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:02:26 by apierret          #+#    #+#             */
/*   Updated: 2025/07/31 21:08:50 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
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
	return (1);
}

static int	process_setup(char *str, int *new_line)
{
	if (str == NULL || new_line == NULL)
		return (0);
	if (str[0] == '-' && str[1] != '\0' && new_line_option(str + 1))
		return (*new_line = 0, 1);
	return (0);
}

static int	write_str_fd(char *str, int fd)
{
	if (str == NULL)
		return (1);
	if (write(fd, str, ft_strlen(str)) == -1)
		return (0);
	return (1);
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
			if (!write_str_fd(data.argv[i], data.stdout))
				return (error(ERR_WRITE, NULL));
			if (i != data.argc -1 && !write_str_fd(" ", data.stdout))
				return (error(ERR_WRITE, NULL));
		}
		i++;
	}
	if (new_line && !write_str_fd("\n", data.stdout))
		return (error(ERR_WRITE, NULL));
	return (*ret = 0, error(ERR_NONE, NULL));
}
