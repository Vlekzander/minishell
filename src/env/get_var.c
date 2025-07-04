/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:16:09 by apierret          #+#    #+#             */
/*   Updated: 2025/07/05 01:12:41 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "utils.h"

extern int	g_signal;

t_error	get_var(char **str, t_hash_table *env, char *key)
{
	char	*value;
	char	sig_str[4];
	t_error	err;

	if (str == NULL || env == NULL || key == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (g_signal != 0)
	{
		sig_str[0] = '\0';
		ft_strlcat(sig_str, "130", 4);
		g_signal = 0;
		err = set_var(env, "?", sig_str);
		if (err.id != ERR_NONE)
			return (err);
	}
	value = htable_get(env, key);
	if (value == NULL)
		value = "";
	return (*str = value, error(ERR_NONE, NULL));
}
