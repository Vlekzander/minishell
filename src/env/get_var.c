/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:16:09 by apierret          #+#    #+#             */
/*   Updated: 2025/06/19 11:34:08 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "utils.h"

t_error	get_var(char **str, t_hash_table *env, char *key)
{
	char	*value;

	if (str == NULL || env == NULL || key == NULL)
		return (ERR_IMPLEMENTATION);
	value = htable_get(env, key);
	return (*str = value, ERR_NONE);
}
