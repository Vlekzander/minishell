/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 15:52:40 by apierret          #+#    #+#             */
/*   Updated: 2025/07/31 13:29:28 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

t_error	quote_removal(char *str, char *mask)
{
	size_t	i;
	size_t	len;

	if (str == NULL || mask == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	i = 0;
	while (str[i] != '\0' && mask[i] != '\0')
	{
		if (mask[i] == 'Q')
		{
			len = ft_strlen(mask + i + 1);
			ft_memmove(str + i, str + i + 1, len + 1);
			ft_memmove(mask + i, mask + i + 1, len + 1);
		}
		else
			i++;
	}
	return (error(ERR_NONE, NULL));
}
