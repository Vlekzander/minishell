/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_strbuilder.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by apierret          #+#    #+#             */
/*   Updated: 2025/06/09 12:31:32 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "data.h"

t_strbuilder	*create_strbuilder(size_t size)
{
	t_strbuilder	*sb;

	if (size == 0)
		return (NULL);
	sb = ft_calloc(1, sizeof(t_strbuilder));
	if (sb == NULL)
		return (NULL);
	sb->buffer = ft_calloc(size, sizeof(char));
	if (sb->buffer == NULL)
		return (free_strbuilder(sb), NULL);
	sb->size = size;
	sb->length = 0;
	return (sb);
}
