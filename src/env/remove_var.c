/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:16:34 by apierret          #+#    #+#             */
/*   Updated: 2025/08/04 19:10:30 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

t_error	remove_var(t_hash_table *env, char *key)
{
	if (env == NULL || key == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (ft_strlen(key) > 1 || ft_strncmp("?", key, 2) != 0)
		htable_remove(env, key);
	return (error(ERR_NONE, NULL));
}
