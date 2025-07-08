/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:05:33 by apierret          #+#    #+#             */
/*   Updated: 2025/07/08 13:09:21 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>
#include "builtins.h"

t_error	builtin_pwd(int *ret, t_btin_data data, t_hash_table *env)
{
	char	path[PATH_MAX];

	if (ret == NULL || data.argv == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (getcwd(path, PATH_MAX) == NULL)
		return (error(ERR_ERRNO, path));
	ft_putendl_fd(path, data.stdout);
	return (*ret = 0, error(ERR_NONE, NULL));
}
