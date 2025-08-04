/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strbuilder_append.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by apierret          #+#    #+#             */
/*   Updated: 2025/08/04 19:19:22 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>
#include "data.h"

int	strbuilder_append(t_strbuilder *sb, char *str)
{
	size_t	str_len;
	char	*buf;

	if (sb == NULL || str == NULL)
		return (0);
	str_len = ft_strlen(str);
	if (sb->length + str_len >= sb->size)
	{
		while (sb->length + str_len >= sb->size)
			sb->size *= 2;
		buf = ft_calloc(sb->size, sizeof(char));
		if (buf == NULL)
			return (0);
		ft_memcpy(buf, sb->buffer, sb->length);
		free(sb->buffer);
		sb->buffer = buf;
	}
	ft_memcpy(sb->buffer + sb->length, str, str_len +1);
	sb->length += str_len;
	return (1);
}
