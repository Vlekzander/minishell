/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 15:25:20 by apierret          #+#    #+#             */
/*   Updated: 2025/07/06 21:20:47 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "builtins.h"
#include "env.h"
#include "libft.h"

static int	str_isdigit(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

t_error	process_errors(int *ret, int is_digit, char *str)
{
	ft_putstr_fd(ERROR_PREFIX, 2);
	ft_putstr_fd(": exit: ", 2);
	if (!is_digit)
	{
		ft_putstr_fd(str, 2);
		ft_putstr_fd(": Numeric argument required\n", 2);
		return (*ret = 2, error(ERR_EXIT, NULL));
	}
	ft_putstr_fd("Too many arguments\n", 2);
	return (*ret = 1, error(ERR_NONE, NULL));
}

t_error	builtin_exit(int *ret, t_btin_data data, t_hash_table *env)
{
	int		is_digit;
	int		value;
	char	*str;

	if (ret == NULL || data.argv == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	ft_putendl_fd("exit", 2);
	if (data.argv[1] != NULL)
	{
		is_digit = str_isdigit(data.argv[1]);
		if (data.argc > 2 || !is_digit)
			return (process_errors(ret, is_digit, data.argv[1]));
	}
	value = 1;
	if (data.argv[1] != NULL)
		value = ft_atoi(data.argv[1]);
	else
	{
		get_var(&str, env, "?");
		if (*str != '\0')
			value = ft_atoi(str);
	}
	return (*ret = value, error(ERR_EXIT, NULL));
}
