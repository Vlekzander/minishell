/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:16:34 by apierret          #+#    #+#             */
/*   Updated: 2025/06/19 11:37:43 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

t_error	remove_var(t_hash_table *env, char *key)
{
	if (env == NULL || key == NULL)
		return (ERR_IMPLEMENTATION);
	htable_remove(env, key);
	return (ERR_NONE);
}
