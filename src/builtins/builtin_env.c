/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 20:49:27 by apierret          #+#    #+#             */
/*   Updated: 2025/07/08 19:08:23 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "builtins.h"
#include "data.h"
#include "env.h"

t_error	builtin_env(int *ret, t_btin_data data, t_hash_table *env)
{
	t_error	err;
	char	*str;

	if (ret == NULL || data.argv == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = get_env(&str, 0, env);
	if (err.id != ERR_NONE)
		return (*ret = 1, err);
	ft_putstr_fd(str, data.stdout);
	free(str);
	return (*ret = 0, error(ERR_NONE, NULL));
}
