/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:16:01 by apierret          #+#    #+#             */
/*   Updated: 2025/05/23 00:06:34 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stddef.h>

#include "env.h"

t_error	get_env(char **str, char **env)
{
	char	*env_str;
	size_t	size;
	int		i;

	if (str == NULL || env == NULL)
		return (ERR_IMPLEMENTATION);
	size = 0;
	i = 0;
	while (env[i] != NULL)
	{
		size += ft_strlen(env[i]) + 1;
		i++;
	}
	env_str = ft_calloc(size +1, sizeof(char));
	if (env_str == NULL)
		return (ERR_ALLOCATION);
	i = 0;
	while (env[i] != NULL)
	{
		ft_strlcat(env_str, env[i], size +1);
		ft_strlcat(env_str, "\n", size +1);
		i++;
	}
	return (*str = env_str, ERR_NONE);
}
