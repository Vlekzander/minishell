/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:18:01 by apierret          #+#    #+#             */
/*   Updated: 2025/07/08 14:30:12 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"

t_error	builtin_unset(int *ret, t_btin_data data, t_hash_table *env)
{
	int		i;

	if (ret == NULL || data.argv == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	i = 1;
	while (i < data.argc)
		remove_var(env, data.argv[i++]);
	return (*ret = 0, error(ERR_NONE, NULL));
}
