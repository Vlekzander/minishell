/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:18:01 by apierret          #+#    #+#             */
/*   Updated: 2025/08/02 19:07:53 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "data.h"
#include "env.h"

t_error	builtin_unset(int *ret, void *data, t_hash_table *env)
{
	t_command	*cmd;
	int			i;

	cmd = (t_command *) data;
	if (ret == NULL || cmd->argv == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	i = 1;
	while (i < cmd->argc)
		remove_var(env, cmd->argv[i++]);
	return (*ret = 0, error(ERR_NONE, NULL));
}
