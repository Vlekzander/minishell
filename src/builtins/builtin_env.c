/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 20:49:27 by apierret          #+#    #+#             */
/*   Updated: 2025/08/04 19:45:02 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "builtins.h"
#include "data.h"
#include "env.h"

t_error	builtin_env(int *ret, void *data, t_hash_table *env)
{
	t_command	*cmd;
	char		*str;
	t_error		err;

	cmd = (t_command *) data;
	if (ret == NULL || cmd->argv == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = get_env_str(&str, 0, env);
	if (err.id != ERR_NONE)
		return (*ret = 1, err);
	ft_putstr_fd(str, cmd->stdout);
	free(str);
	return (*ret = 0, error(ERR_NONE, NULL));
}
