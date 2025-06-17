/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   htable_hash.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 23:44:28 by apierret          #+#    #+#             */
/*   Updated: 2025/06/16 23:46:24 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

size_t	htable_hash(char *key)
{
	size_t	hash;
	size_t	i;

	hash = 5381;
	if (key == NULL)
		return (hash);
	i = 0;
	while (key[i] != '\0')
	{
		hash = (hash << 5) + hash + key[i];
		i++;
	}
	return (hash);
}
