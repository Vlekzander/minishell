/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 15:25:20 by apierret          #+#    #+#             */
/*   Updated: 2025/07/15 00:53:24 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>

#include "builtins.h"
#include "env.h"
#include "libft.h"
#define EXIT_MAX "9223372036854775807"
#define EXIT_MIN "9223372036854775808"

static char	*str_trim(char *s1, char *set)
{
	size_t	start;
	size_t	len;

	if (s1 == NULL || set == NULL)
		return (NULL);
	start = 0;
	while (s1[start] != '\0' && ft_strchr(set, s1[start]))
		start++;
	len = ft_strlen(s1 + start);
	while (len > 0 && ft_strchr(set, s1[start + len -1]))
		len--;
	s1[start + len] = '\0';
	return (s1 + start);
}

static int	str_isdigit(char *str)
{
	int		i;
	int		start;
	char	*limit;

	if (str == NULL)
		return (0);
	limit = EXIT_MAX;
	i = 0;
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i] != '\0' && str[i] == '0')
		i++;
	if (str[0] == '-')
		limit = EXIT_MIN;
	start = i;
	while (str[i] != '\0' && ft_isdigit(str[i]))
		i++;
	if (str[i] != '\0' || ft_strlen(str + start) > ft_strlen(limit))
		return (0);
	if (ft_strlen(str + start) < ft_strlen(limit))
		return (1);
	if (ft_strncmp(limit, str + start, ft_strlen(limit) +1) < 0)
		return (0);
	return (1);
}

static t_error	check_str(int *ret, t_btin_data *data)
{
	int		is_digit;
	char	*str;

	str = str_trim(data->argv[1], " \t\n");
	is_digit = str_isdigit(data->argv[1]);
	if (!is_digit)
	{
		print_error(error(ERR_NUMERIC_ARG, str), "exit");
		return (*ret = 2, error(ERR_EXIT, NULL));
	}
	if (data->argc > 2)
	{
		print_error(error(ERR_TOO_MANY_ARGS, NULL), "exit");
		return (*ret = 1, error(ERR_NONE, NULL));
	}
	return (error(ERR_NONE, NULL));
}

t_error	builtin_exit(int *ret, t_btin_data data, t_hash_table *env)
{
	int		value;
	char	*str;
	t_error	err;

	if (ret == NULL || data.argv == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (!data.forked)
		ft_putendl_fd("exit", 2);
	if (data.argv[1] != NULL)
	{
		err = check_str(&value, &data);
		if (err.id != ERR_NONE || data.argc > 2)
			return (*ret = value, err);
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
