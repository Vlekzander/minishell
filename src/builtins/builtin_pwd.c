/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:05:33 by apierret          #+#    #+#             */
/*   Updated: 2025/08/03 18:50:43 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>
#include "builtins.h"
#include "data.h"

t_error	builtin_pwd(int *ret, void *data, t_hash_table *env)
{
	t_command	*cmd;
	char		path[PATH_MAX];

	cmd = (t_command *) data;
	if (ret == NULL || cmd->argv == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (getcwd(path, PATH_MAX) == NULL)
		return (error(ERR_ERRNO, path));
	ft_putendl_fd(path, cmd->stdout);
	return (*ret = 0, error(ERR_NONE, NULL));
}
