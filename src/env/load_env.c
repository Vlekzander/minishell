/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:15:52 by apierret          #+#    #+#             */
/*   Updated: 2025/05/22 19:14:16 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "env.h"
#include "libft.h"
#include "utils.h"

t_error	load_env(char ***env, char **envp)
{
	char	**array;
	int		i;
	t_error	error;

	if (env == NULL || envp == NULL)
		return (ERR_IMPLEMENTATION);
	array = ft_calloc(1, sizeof(char *));
	if (array == NULL)
		return (ERR_ALLOCATION);
	i = 0;
	while (envp[i] != NULL)
	{
		error = str_array_push(&array, envp[i]);
		if (error != ERR_NONE)
			return (free_ddarray((void **) array), error);
		i++;
	}
	return (*env = array, ERR_NONE);
}
