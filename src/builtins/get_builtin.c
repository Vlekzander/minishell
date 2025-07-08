/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 15:22:48 by apierret          #+#    #+#             */
/*   Updated: 2025/07/08 10:59:44 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

#include "builtins.h"
#include "libft.h"

t_btin	get_builtin(char *name)
{
	if (name == NULL)
		return (NULL);
	if (ft_strncmp("cd", name, 3) == 0)
		return (builtin_cd);
	if (ft_strncmp("echo", name, 5) == 0)
		return (builtin_echo);
	if (ft_strncmp("env", name, 4) == 0)
		return (builtin_env);
	if (ft_strncmp("exit", name, 5) == 0)
		return (builtin_exit);
	if (ft_strncmp("pwd", name, 4) == 0)
		return (builtin_pwd);
	return (NULL);
}
